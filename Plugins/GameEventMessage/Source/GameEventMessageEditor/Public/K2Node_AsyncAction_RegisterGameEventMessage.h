// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_AsyncAction.h"
#include "K2Node_AsyncAction_RegisterGameEventMessage.generated.h"

/**
 * 
 */
UCLASS()
class GAMEEVENTMESSAGEEDITOR_API UK2Node_AsyncAction_RegisterGameEventMessage : public UK2Node_AsyncAction
{
	GENERATED_BODY()

private:
	// 노드가 재구성 될때 (핀 변경 같은 다시 동기화)
	virtual void PostReconstructNode() override;
	
	// 특정 핀의 기본 값이 에디터에서 변경될때
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;

	// 에디터에서 특정 핀 위에 마우스 올렸을때 툴팁 확장하는거
	virtual void GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const override;

	// 블루프린트 액션 메뉴 팔레트에 노드를 등록하는 메소드
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	// 노드 생성시 기본 핀을 구성하는 메소드
	virtual void AllocateDefaultPins() override;

protected:
	// 부모(UK2Node_AsyncAction)이 비동기 Delegate를 구성할때 호출 OnMessageReceived 등등
	// 블루프린트 컴파일시 Iterator를 순회해서 Wildcard에 연결
	virtual bool HandleDelegates(const TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs, UEdGraphPin* ProxyObjectPin, UEdGraphPin*& InOutLastThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext) override;
private:
	// HandleDelegate에서 호출 Payload연결
	bool HandlePayloadImplementation(
		FMulticastDelegateProperty* CurrentProperty,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ProxyObjectVar,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& PayloadVar,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ActualChannelVar,
		UEdGraphPin*& InOutLastActivatedThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext);

private:
	void RefreshOutputPin();

	UEdGraphPin* GetPayloadPin() const;
	UEdGraphPin* GetPayloadTypePin() const;
	UEdGraphPin* GetOutputChannelPin() const;
};
