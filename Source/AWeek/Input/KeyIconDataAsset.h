// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KeyIconDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UKeyIconDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="KeyIcon")
	TMap<FKey, TSoftObjectPtr<UTexture2D>> KeyToIcon;

	// 키 이름으로 바로 찾기 (동기 로드)
	UFUNCTION(BlueprintCallable, Category="KeyIcon")
	UTexture2D* GetIconByName_Sync(FName KeyName) const;

	// FKey로 찾기 (동기 로드)
	UFUNCTION(BlueprintCallable, Category="KeyIcon")
	UTexture2D* GetIconByKey_Sync(FKey Key) const;

	// 비동기 로드 (완료 시 콜백)
	void GetIconByKey_Async(FKey Key, TFunction<void(UTexture2D*)> OnLoaded) const;
};
