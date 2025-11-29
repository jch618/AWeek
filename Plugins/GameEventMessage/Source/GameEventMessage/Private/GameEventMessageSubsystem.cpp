// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEventMessageSubsystem.h"

void FGameEventMessageListenerHandle::Unregister()
{
	if (UGameEventMessageSubsystem* StrongSubsystem = Subsystem.Get())
	{
		StrongSubsystem->UnregisterListener(*this);
		Subsystem.Reset();
		Channel = FGameplayTag();
		ID = 0;
	}
}

UGameEventMessageSubsystem& UGameEventMessageSubsystem::Get(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	check(World);
	UGameEventMessageSubsystem* Router = UGameInstance::GetSubsystem<UGameEventMessageSubsystem>(World->GetGameInstance());
	check(Router);

	return *Router;
}

bool UGameEventMessageSubsystem::HasInstance(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	const UGameEventMessageSubsystem* Router = World != nullptr ? UGameInstance::GetSubsystem<UGameEventMessageSubsystem>(World->GetGameInstance()) : nullptr;
	return Router != nullptr;
}

void UGameEventMessageSubsystem::Deinitialize()
{
	ListenerMap.Reset();
	
	Super::Deinitialize();
}

void UGameEventMessageSubsystem::UnregisterListener(FGameEventMessageListenerHandle Handle)
{
	if (Handle.IsValid())
	{
		check(Handle.Subsystem == this);
		UnRegisterListenerInternal(Handle.Channel, Handle.ID);
	}
}

void UGameEventMessageSubsystem::K2_BroadcastMessage(FGameplayTag Channel, const int32& Message)
{
	checkNoEntry();
}

DEFINE_FUNCTION(UGameEventMessageSubsystem::execK2_BroadcastMessage)
{
	P_GET_STRUCT(FGameplayTag, Channel);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	void* MessagePtr = Stack.MostRecentPropertyAddress;
	FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

	P_FINISH;

	if (ensure((StructProp != nullptr) && (StructProp->Struct != nullptr) && (MessagePtr != nullptr)))
	{
		P_THIS->BroadcastMessageInternal(Channel, StructProp->Struct, MessagePtr);
	}
}

void UGameEventMessageSubsystem::BroadcastMessageInternal(FGameplayTag Channel, const UScriptStruct* StructType,
                                                          const void* MessageBytes)
{
	bool bOnInitialTag = true;
	for (FGameplayTag Tag = Channel; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FChannelListenerList* ChannelListenerList = ListenerMap.Find(Tag))
		{
			TArray<FGameEventMessageListenerData> ListenerArray(ChannelListenerList->Listeners);

			for (const FGameEventMessageListenerData& Listener:ListenerArray)
			{
				if (bOnInitialTag || (Listener.MatchType == EGameEventMessageMatch::PartialMatch))
				{
					if (Listener.bHadValidType && !Listener.ListenerStructType.IsValid())
					{
						UE_LOG(LogTemp, Warning, TEXT("리스너 구조체 유형이 유효하지 않음 %s 목록에서 리스너를 제거 진행"), *Channel.ToString());
						UnRegisterListenerInternal(Channel, Listener.HandleID);
						continue;
					}

					if (!Listener.bHadValidType || StructType->IsChildOf(Listener.ListenerStructType.Get()))
					{
						Listener.ReceiveCallback(Channel, StructType, MessageBytes);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Struct Type이 맞지 않음 channel %s (broadcast type %s, listener at %s was expecting type %s)"),
						*Channel.ToString(),
						*StructType->GetPathName(),
						*Tag.ToString(),
						*Listener.ListenerStructType->GetPathName());
					}
				}
			}
		}

		bOnInitialTag = false;
	}
}

FGameEventMessageListenerHandle UGameEventMessageSubsystem::RegisterListenerInternal(FGameplayTag Channel,
	TFunction<void(FGameplayTag, const UScriptStruct*, const void*)>&& Callback, const UScriptStruct* StructType,
	EGameEventMessageMatch MatchType)
{
	FChannelListenerList& List = ListenerMap.FindOrAdd(Channel);

	FGameEventMessageListenerData& Entry = List.Listeners.AddDefaulted_GetRef();
	Entry.ReceiveCallback = MoveTemp(Callback);
	Entry.ListenerStructType = StructType;
	Entry.bHadValidType = StructType != nullptr; // 비어있으면 무조건 실행하게 bHadValidType이 false면
	Entry.HandleID = ++List.HandleID;
	Entry.MatchType = MatchType;

	return FGameEventMessageListenerHandle(this, Channel, Entry.HandleID);
}

void UGameEventMessageSubsystem::UnRegisterListenerInternal(FGameplayTag Channel, int32 HandleID)
{
	if (FChannelListenerList* ChannelListenerList = ListenerMap.Find(Channel))
	{
		int32 MatchIndex = ChannelListenerList->Listeners.IndexOfByPredicate([ID = HandleID](const FGameEventMessageListenerData& Other) { return Other.HandleID == ID; });
		if (MatchIndex != INDEX_NONE)
		{
			ChannelListenerList->Listeners.RemoveAtSwap(MatchIndex);
		}

		if (ChannelListenerList->Listeners.Num() == 0)
		{
			ListenerMap.Remove(Channel);
		}
	}
}
