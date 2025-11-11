// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyIconDataAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

UTexture2D* UKeyIconDataAsset::GetIconByName_Sync(FName KeyName) const
{
    return GetIconByKey_Sync(FKey(KeyName));
}

UTexture2D* UKeyIconDataAsset::GetIconByKey_Sync(FKey Key) const
{
    if (const TSoftObjectPtr<UTexture2D>* Found = KeyToIcon.Find(Key))
    {
        return Found->LoadSynchronous();
    }
    
    return nullptr;
}

void UKeyIconDataAsset::GetIconByKey_Async(FKey Key, TFunction<void(UTexture2D*)> OnLoaded) const
{
    if (const TSoftObjectPtr<UTexture2D>* Found = KeyToIcon.Find(Key.GetFName()))
     {
         if (Found->IsNull())
         {
             if (OnLoaded) OnLoaded(nullptr);
             return;
         }
 
         if (Found->IsValid())
         {
             if (OnLoaded) OnLoaded(Found->Get());
             return;
         }
 
         FStreamableManager& SM = UAssetManager::GetStreamableManager();
         SM.RequestAsyncLoad(Found->ToSoftObjectPath(),
             [Found, OnLoaded]()
             {
                 UTexture2D* Tex = Found->Get();
                 if (OnLoaded) OnLoaded(Tex);
             });
     }
     else
     {
         if (OnLoaded) OnLoaded(nullptr);
     }
}
