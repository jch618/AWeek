// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEventMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "Engine/CancellableAsyncAction.h"
#include "AsyncAction_RegisterGameEventMessage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncGameEventMessageDelegate, UAsyncAction_RegisterGameEventMessage*, Proxy, FGameplayTag, ActualChannel);
/**
 * 
 */
UCLASS(BlueprintType)
class AWEEK_API UAsyncAction_RegisterGameEventMessage : public UCancellableAsyncAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category=EvenetMessage, meta=(WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static UAsyncAction_RegisterGameEventMessage* RegisterGameEventMessage(UObject* WorldContextObject, FGameplayTag Channel, UScriptStruct* PayloadType,
		EGameEventMessageMatch MatchType = EGameEventMessageMatch::ExactMatch);

	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;

	UPROPERTY(BlueprintAssignable)
	FAsyncGameEventMessageDelegate OnReceived;

private:
	void HandleMessageReceived(FGameplayTag Channel, const UScriptStruct* StructType, const void* Payload);

	const void* ReceivedMessagePayloadPtr = nullptr;
	TWeakObjectPtr<UWorld> WorldPtr;
	FGameplayTag ActualChannel;
	TWeakObjectPtr<UScriptStruct> MessageStructType = nullptr;
	EGameEventMessageMatch MatchType = EGameEventMessageMatch::ExactMatch;

	FGameEventMessageListenerHandle ListenerHandle;
};
