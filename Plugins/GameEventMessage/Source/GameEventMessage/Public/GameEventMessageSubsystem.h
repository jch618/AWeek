// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameEventMessageSubsystem.generated.h"

class UAsyncAction_RegisterGameEventMessage;
class UGameEventMessageSubsystem;

UENUM(BlueprintType)
enum class EGameEventMessageMatch : uint8
{
	ExactMatch,
	PartialMatch
};

USTRUCT(BlueprintType)
struct GAMEEVENTMESSAGE_API FGameEventMessageListenerHandle
{
	GENERATED_BODY()

public:
	FGameEventMessageListenerHandle()
	{
	}

	bool IsValid() const { return ID != 0; }

	void Unregister();
private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UGameEventMessageSubsystem> Subsystem;

	UPROPERTY(Transient)
	FGameplayTag Channel;

	UPROPERTY(Transient)
	int32 ID = 0;

	friend UGameEventMessageSubsystem;

	FGameEventMessageListenerHandle(UGameEventMessageSubsystem* InSubsystem, FGameplayTag InChannel, int32 InID)
		: Subsystem(InSubsystem)
		  , Channel(InChannel)
		  , ID(InID)
	{
	}
};

USTRUCT()
struct FGameEventMessageListenerData
{
	GENERATED_BODY()

public:
	TFunction<void(FGameplayTag, const UScriptStruct*, const void*)> ReceiveCallback;

	int32 HandleID;
	EGameEventMessageMatch MatchType;

	TWeakObjectPtr<const UScriptStruct> ListenerStructType = nullptr;
	bool bHadValidType = false;
};

/**
 * 
 */
UCLASS()
class GAMEEVENTMESSAGE_API UGameEventMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	friend UAsyncAction_RegisterGameEventMessage;
public:
	static UGameEventMessageSubsystem& Get(const UObject* WorldContextObject);
	static bool HasInstance(const UObject* WorldContextObject);

	virtual void Deinitialize() override;

	template <typename FMessageStructType>
	void BroadcastMessage(const FGameplayTag Channel, const FMessageStructType& Message)
	{
		const UScriptStruct* StructType = TBaseStructure<FMessageStructType>::Get();
		BroadcastMessageInternal(Channel, StructType, &Message);
	}

	template <typename FMessageStructType>
	FGameEventMessageListenerHandle RegisterListener(FGameplayTag Channel,
	                                                 TFunction<void(FGameplayTag, const FMessageStructType&)>&&
	                                                 Callback,
	                                                 EGameEventMessageMatch MatchType =
		                                                 EGameEventMessageMatch::ExactMatch)
	{
		auto ChangingCallback = [InnerCallback = MoveTemp(Callback)](FGameplayTag Channel,
		                                                             const UScriptStruct* SenderStructType,
		                                                             const void* SenderPayload)
		{
			InnerCallback(Channel, *static_cast<const FMessageStructType*>(SenderPayload));
		};

		const UScriptStruct* StructType = TBaseStructure<FMessageStructType>::Get();
		return RegisterListenerInternal(Channel, ChangingCallback, StructType, MatchType);
	}

	void UnregisterListener(FGameEventMessageListenerHandle Handle);

protected:
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam="Message", AllowAbstract="false", DisplayName="Broadcast Message"))
	void K2_BroadcastMessage(FGameplayTag Channel, const int32& Message);

	DECLARE_FUNCTION(execK2_BroadcastMessage);

private:
	void BroadcastMessageInternal(FGameplayTag Channel, const UScriptStruct* StructType, const void* MessageBytes);

	FGameEventMessageListenerHandle RegisterListenerInternal(FGameplayTag Channel,
	                                                         TFunction<void(
		                                                         FGameplayTag, const UScriptStruct*,
		                                                         const void*)>&& Callback,
	                                                         const UScriptStruct* StructType,
	                                                         EGameEventMessageMatch MatchType);

	void UnRegisterListenerInternal(FGameplayTag Channel, int32 HandleID);

	struct FChannelListenerList
	{
		TArray<FGameEventMessageListenerData> Listeners;
		int32 HandleID = 0;
	};

	TMap<FGameplayTag, FChannelListenerList> ListenerMap;
};
