// Fill out your copyright notice in the Description page of Project Settings.


#include "UIExtensionSystem.h"

#include "Blueprint/UserWidget.h"

void FUIExtensionHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtension(*this);
	}
}

bool FUIExtensionPoint::DoesExtensionPassContract(const FUIExtension* Extension) const
{
	if (UObject* DataPtr = Extension->Data)
	{
		const bool bMatchContext = (ContextObject.IsExplicitlyNull() && Extension->ContextObject.IsExplicitlyNull())
			|| (ContextObject == Extension->ContextObject);

		if (bMatchContext)
		{
			const UClass* DataClass = DataPtr->IsA(UClass::StaticClass()) ? Cast<UClass>(DataPtr) : DataPtr->GetClass();
			for (const UClass* AllowedDataClass : AllowedDataClasses)
			{
				if (DataClass->IsChildOf(AllowedDataClass) || DataClass->ImplementsInterface(AllowedDataClass))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void FUIExtensionPointHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtensionPoint(*this);
	}
}

void UUIExtensionSubsystem::UnregisterExtension(const FUIExtensionHandle& ExtensionHandle)
{
	if (ExtensionHandle.IsValid())
	{
		// handle이 UIExtensionSubsystem이랑 같은지
		checkf(ExtensionHandle.ExtensionSource == this, TEXT("not from this extension subsystem!!!!!"));

		TSharedPtr<FUIExtension> Extension = ExtensionHandle.DataPtr;
		if (FExtensionList* ListPtr = ExtensionMap.Find(Extension->ExtensionPointTag))
		{
			NotifyExtensionPointOfExtension(EUIExtensionAction::Removed, Extension);
			ListPtr->RemoveSwap(Extension);
			if (ListPtr->Num() == 0)
			{
				ExtensionMap.Remove(Extension->ExtensionPointTag);
			}
		}
	}
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag,
                                                                              UObject* ContextObject,
                                                                              TSubclassOf<UUserWidget> WidgetClass,
                                                                              int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, ContextObject, WidgetClass, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag,
                                                                  UObject* ContextObject, UObject* Data, int32 Priority)
{
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionHandle();
	}

	if (!Data)
	{
		return FUIExtensionHandle();
	}

	FExtensionList& List = ExtensionMap.FindOrAdd(ExtensionPointTag);

	TSharedPtr<FUIExtension>& Entry = List.Add_GetRef(MakeShared<FUIExtension>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->Data = Data;
	Entry->Priority = Priority;

	NotifyExtensionPointOfExtension(EUIExtensionAction::Added, Entry);

	return FUIExtensionHandle(this, Entry);
}

FUIExtensionRequest UUIExtensionSubsystem::CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension)
{
	FUIExtensionRequest Request;
	Request.ExtensionHandle = FUIExtensionHandle(this, Extension);
	Request.ExtensionPointTag = Extension->ExtensionPointTag;
	Request.Priority = Extension->Priority;
	Request.Data = Extension->Data;
	Request.ContextObject = Extension->ContextObject.Get();
	return Request;
}

void UUIExtensionSubsystem::UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle)
{
	if (ExtensionPointHandle.IsValid())
	{
		check(ExtensionPointHandle.ExtensionSource == this);

		const TSharedPtr<FUIExtensionPoint> ExtensionPoint = ExtensionPointHandle.DataPtr;
		if (FExtensionPointList* ListPtr = ExtensionPointMap.Find(ExtensionPoint->ExtensionPointTag))
		{
			 ListPtr->RemoveSwap(ExtensionPoint);
			if (ListPtr->Num() == 0)
			{
				ExtensionPointMap.Remove(ExtensionPoint->ExtensionPointTag);
			}
		}
	}
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag,
	UObject* ContextObject, EUIExtensionPointMatch ExtensionPointTagMatchType,
	const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionPointHandle();
	}

	if (!ExtensionCallback.IsBound())
	{
		return FUIExtensionPointHandle();
	}

	if (AllowedDataClasses.Num() == 0)
	{
		return FUIExtensionPointHandle();
	}

	FExtensionPointList& List = ExtensionPointMap.FindOrAdd(ExtensionPointTag);
	TSharedPtr<FUIExtensionPoint>& Entry = List.Add_GetRef(MakeShared<FUIExtensionPoint>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->ExtensionPointTagMatchType = ExtensionPointTagMatchType;
	Entry->AllowedDataClasses = AllowedDataClasses;
	Entry->Callback = MoveTemp(ExtensionCallback);
	
	NotifyExtensionPointOfExtensions(Entry);
	
	return FUIExtensionPointHandle(this, Entry);
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag,
                                                                      EUIExtensionPointMatch ExtensionPointTagMatchType,
                                                                      const TArray<UClass*>& AllowedDataClasses,
                                                                      FExtendExtensionPointDelegate ExtensionCallback)
{
	return RegisterExtensionPointForContext(ExtensionPointTag, nullptr, ExtensionPointTagMatchType, AllowedDataClasses,
	                                        ExtensionCallback);
}

void UUIExtensionSubsystem::NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint)
{
	for (FGameplayTag Tag = ExtensionPoint->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionList* ListPtr = ExtensionMap.Find(Tag))
		{
			FExtensionList ExtensionArray(*ListPtr);
			for (const TSharedPtr<FUIExtension>& Extension : ExtensionArray)
			{
				if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
				{
					FUIExtensionRequest Request = CreateExtensionRequest(Extension);
					ExtensionPoint->Callback.ExecuteIfBound(EUIExtensionAction::Added, Request);
				}
			}
		}
	}
}

void UUIExtensionSubsystem::NotifyExtensionPointOfExtension(EUIExtensionAction Action,
                                                            TSharedPtr<FUIExtension>& Extension)
{
	bool bOnInitialTag = true;
	for (FGameplayTag Tag = Extension->ExtensionPointTag;Tag.IsValid();Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionPointList* ListPtr = ExtensionPointMap.Find(Tag))
		{
			FExtensionPointList ExtensionPointArray(*ListPtr);
			for (const TSharedPtr<FUIExtensionPoint>& ExtensionPoint: ExtensionPointArray)
			{
				if (bOnInitialTag || (ExtensionPoint->ExtensionPointTagMatchType == EUIExtensionPointMatch::PartialMatch))
				{
					if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
					{
						FUIExtensionRequest Request = CreateExtensionRequest(Extension);
						ExtensionPoint->Callback.ExecuteIfBound(Action, Request);
					}
				}
			}
		}
		
		bOnInitialTag = false;
	}
}
