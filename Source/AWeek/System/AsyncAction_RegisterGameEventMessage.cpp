// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction_RegisterGameEventMessage.h"

UAsyncAction_RegisterGameEventMessage* UAsyncAction_RegisterGameEventMessage::RegisterGameEventMessage(
	UObject* WorldContextObject, FGameplayTag Channel, UScriptStruct* PayloadType, EGameEventMessageMatch MatchType)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	UAsyncAction_RegisterGameEventMessage* Action = NewObject<UAsyncAction_RegisterGameEventMessage>();
	Action->WorldPtr = World;
	Action->ActualChannel = Channel;
	Action->MessageStructType = PayloadType;
	Action->MatchType = MatchType;
	Action->RegisterWithGameInstance(World);

	return Action;
}

void UAsyncAction_RegisterGameEventMessage::Activate()
{
	if (UWorld* World = WorldPtr.Get())
	{
		if (UGameEventMessageSubsystem::HasInstance(World))
		{
			UGameEventMessageSubsystem& Router = UGameEventMessageSubsystem::Get(World);
			TWeakObjectPtr WeakThis(this);
			ListenerHandle = Router.RegisterListenerInternal(ActualChannel,
				[WeakThis](FGameplayTag Channel, const UScriptStruct* StructType, const void* Payload)
				{
					if (UAsyncAction_RegisterGameEventMessage* StrongThis = WeakThis.Get())
					{
						StrongThis->HandleMessageReceived(Channel, StructType, Payload);
					}	
				}, MessageStructType.Get(), MatchType);

			return;
		}
	}

	SetReadyToDestroy();
}

void UAsyncAction_RegisterGameEventMessage::SetReadyToDestroy()
{
	ListenerHandle.Unregister();
	Super::SetReadyToDestroy();
}

void UAsyncAction_RegisterGameEventMessage::HandleMessageReceived(FGameplayTag Channel, const UScriptStruct* StructType,
	const void* Payload)
{
	if (!MessageStructType.Get() || (MessageStructType.Get() == StructType))
	{
		ReceivedMessagePayloadPtr = Payload;
		OnReceived.Broadcast(this, Channel);
		ReceivedMessagePayloadPtr = nullptr;
	}

	if (!OnReceived.IsBound())
	{
		SetReadyToDestroy();
	}
}
