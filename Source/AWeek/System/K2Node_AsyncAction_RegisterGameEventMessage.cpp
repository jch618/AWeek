// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_AsyncAction_RegisterGameEventMessage.h"

#include "AsyncAction_RegisterGameEventMessage.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFunctionNodeSpawner.h"
#include "K2Node_AssignmentStatement.h"
#include "K2Node_CallFunction.h"
#include "K2Node_TemporaryVariable.h"
#include "KismetCompiler.h"

namespace UK2Node_AsyncAction_RegisterGameEventMessagesName
{
	static FName PayloadPinName = "Payload";
	static FName PayloadTypePinName = "PayloadType";
	static FName ActualChannelPinName = "ActualChannel";
	static FName ProxyPinName = "Proxy";
}

void UK2Node_AsyncAction_RegisterGameEventMessage::PostReconstructNode()
{
	Super::PostReconstructNode();

	RefreshOutputPin();
}

void UK2Node_AsyncAction_RegisterGameEventMessage::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::PinDefaultValueChanged(Pin);

	// PayloadType이 변경됐을때 갱신
	if (Pin == GetPayloadTypePin())
	{
		RefreshOutputPin();
	}
}

void UK2Node_AsyncAction_RegisterGameEventMessage::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	Super::GetPinHoverText(Pin, HoverTextOut);
}

void UK2Node_AsyncAction_RegisterGameEventMessage::GetMenuActions(
	FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// UK2Node_AsyncAction 부모 를래스 GetMenuAction에서 타입만 바꾼거 
	struct GetMenuActions_Utils
	{
		static void SetNodeFunc(UEdGraphNode* NewNode, bool /*bIsTemplateNode*/, TWeakObjectPtr<UFunction> FunctionPtr)
		{
			UK2Node_AsyncAction_RegisterGameEventMessage* AsyncTaskNode = CastChecked<UK2Node_AsyncAction_RegisterGameEventMessage>(NewNode);
			if (FunctionPtr.IsValid())
			{
				UFunction* Func = FunctionPtr.Get();
				FObjectProperty* ReturnProp = CastFieldChecked<FObjectProperty>(Func->GetReturnProperty());
						
				AsyncTaskNode->ProxyFactoryFunctionName = Func->GetFName();
				AsyncTaskNode->ProxyFactoryClass        = Func->GetOuterUClass();
				AsyncTaskNode->ProxyClass               = ReturnProp->PropertyClass;
			}
		}
	};

	UClass* NodeClass = GetClass();
	ActionRegistrar.RegisterClassFactoryActions<UAsyncAction_RegisterGameEventMessage>(FBlueprintActionDatabaseRegistrar::FMakeFuncSpawnerDelegate::CreateLambda([NodeClass](const UFunction* FactoryFunc)->UBlueprintNodeSpawner*
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintFunctionNodeSpawner::Create(FactoryFunc);
		check(NodeSpawner != nullptr);
		NodeSpawner->NodeClass = NodeClass;

		TWeakObjectPtr<UFunction> FunctionPtr = MakeWeakObjectPtr(const_cast<UFunction*>(FactoryFunc));
		NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(GetMenuActions_Utils::SetNodeFunc, FunctionPtr);

		return NodeSpawner;
	}) );
}

void UK2Node_AsyncAction_RegisterGameEventMessage::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// WildCard 형식으로 Pin 생성 Payload 타입이 변경되므로
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UK2Node_AsyncAction_RegisterGameEventMessagesName::PayloadPinName);

	UEdGraphPin* ProxyPin = FindPin(UK2Node_AsyncAction_RegisterGameEventMessagesName::ProxyPinName);
	if (ProxyPin)
	{
		ProxyPin->bHidden = true;
	}
}

bool UK2Node_AsyncAction_RegisterGameEventMessage::HandleDelegates(
	const TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs, UEdGraphPin* ProxyObjectPin,
	UEdGraphPin*& InOutLastThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext)
{
	// 부모 HandleDelegates 함수 그대로 +) 확장 HandleDelegateImplementation 추가
	bool bIsErrorFree = true;
	for (TFieldIterator<FMulticastDelegateProperty> PropertyIt(ProxyClass); PropertyIt && bIsErrorFree; ++PropertyIt)
	{
		UEdGraphPin* LastActivatedThenPin = nullptr;
		bIsErrorFree &= FBaseAsyncTaskHelper::HandleDelegateImplementation(*PropertyIt, VariableOutputs, ProxyObjectPin, InOutLastThenPin, LastActivatedThenPin, this, SourceGraph, CompilerContext);
		bIsErrorFree &= HandlePayloadImplementation(*PropertyIt, VariableOutputs[0], VariableOutputs[2], VariableOutputs[1], LastActivatedThenPin, SourceGraph, CompilerContext);
	}
	return bIsErrorFree;
}

bool UK2Node_AsyncAction_RegisterGameEventMessage::HandlePayloadImplementation(
	FMulticastDelegateProperty* CurrentProperty, const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ProxyObjectVar,
	const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& PayloadVar,
	const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ActualChannelVar, UEdGraphPin*& InOutLastActivatedThenPin,
	UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext)
{
	bool bIsErrorFree = true;
	const UEdGraphPin* PayloadPin = GetPayloadPin();
	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

	check(CurrentProperty && SourceGraph && Schema);

	const FEdGraphPinType& PinType = PayloadPin->PinType;

	// PinType이 아직 Wildcard면 Refresh가 안됐다는 뜻 그래서 설정 x 바로 return
	if (PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
	{
		// 연결 된게 없으면 그냥 무시
		if (PayloadPin->LinkedTo.Num() == 0)
		{
			return true;
		}

		// 연결 된게 있으면 실행되면 안되므로 error - return false
		return false;
	}

	// Payload 담을 임시 노드 생성
	UK2Node_TemporaryVariable* TempVarOutput = CompilerContext.SpawnInternalVariable(
		this, PinType.PinCategory, PinType.PinSubCategory, PinType.PinSubCategoryObject.Get(),
		PinType.ContainerType, PinType.PinValueType);

	
	UK2Node_CallFunction* const CallGetPayloadNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallGetPayloadNode->FunctionReference.SetExternalMember(TEXT("GetPayload"), CurrentProperty->GetOwnerClass());
	CallGetPayloadNode->AllocateDefaultPins();

	UEdGraphPin* GetPayloadCallSelfPin = Schema->FindSelfPin(*CallGetPayloadNode, EGPD_Input);
	if (GetPayloadCallSelfPin)
	{
		bIsErrorFree &= Schema->TryCreateConnection(GetPayloadCallSelfPin, ProxyObjectVar.TempVar->GetVariablePin());

		UEdGraphPin* GetPayloadExecPin = CallGetPayloadNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
		UEdGraphPin* GetPayloadThenPin = CallGetPayloadNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);

		UEdGraphPin* GetPayloadPin = CallGetPayloadNode->FindPinChecked(TEXT("OutPayload"));
		bIsErrorFree &= Schema->TryCreateConnection(TempVarOutput->GetVariablePin(), GetPayloadPin);

		UK2Node_AssignmentStatement* AssignNode = CompilerContext.SpawnIntermediateNode<UK2Node_AssignmentStatement>(this, SourceGraph);
		AssignNode->AllocateDefaultPins();
		bIsErrorFree &= Schema->TryCreateConnection(GetPayloadThenPin, AssignNode->GetExecPin());
		bIsErrorFree &= Schema->TryCreateConnection(PayloadVar.TempVar->GetVariablePin(), AssignNode->GetVariablePin());
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetVariablePin());
		bIsErrorFree &= Schema->TryCreateConnection(AssignNode->GetValuePin(), TempVarOutput->GetVariablePin());
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetValuePin());


		bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*InOutLastActivatedThenPin, *AssignNode->GetThenPin()).CanSafeConnect();
		bIsErrorFree &= Schema->TryCreateConnection(InOutLastActivatedThenPin, GetPayloadExecPin);

		UEdGraphPin* OutActualChannelPin = GetOutputChannelPin();
		bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*OutActualChannelPin, *ActualChannelVar.TempVar->GetVariablePin()).CanSafeConnect();
	}

	return bIsErrorFree;
}

void UK2Node_AsyncAction_RegisterGameEventMessage::RefreshOutputPin()
{
	UEdGraphPin* PayloadPin = GetPayloadPin();
	UEdGraphPin* PayloadTypePin = GetPayloadTypePin();

	// 현재 Payload 타입과 출력하고자하는 PayloadType이 다르면 갱신
	if (PayloadPin->PinType.PinSubCategoryObject != PayloadTypePin->DefaultObject)
	{
		// 에디터 상에서 Payload가 분해돼서 사용되는 중이면 Recombine 후 타입 변경
		if (PayloadPin->SubPins.Num() > 0)
		{
			GetSchema()->RecombinePin(PayloadPin);
		}

		PayloadPin->PinType.PinSubCategoryObject = PayloadTypePin->DefaultObject;
		PayloadPin->PinType.PinCategory = (PayloadTypePin->DefaultObject == nullptr)?
		UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Struct;
	}
}

UEdGraphPin* UK2Node_AsyncAction_RegisterGameEventMessage::GetPayloadPin() const
{
	UEdGraphPin* PayloadPin = FindPinChecked(UK2Node_AsyncAction_RegisterGameEventMessagesName::PayloadPinName);
	check(PayloadPin->Direction == EGPD_Output)

	return PayloadPin;
}

UEdGraphPin* UK2Node_AsyncAction_RegisterGameEventMessage::GetPayloadTypePin() const
{
	UEdGraphPin* PayloadTypePin = FindPinChecked(UK2Node_AsyncAction_RegisterGameEventMessagesName::PayloadTypePinName);
	check(PayloadTypePin->Direction == EGPD_Input)

	return PayloadTypePin;
}

UEdGraphPin* UK2Node_AsyncAction_RegisterGameEventMessage::GetOutputChannelPin() const
{
	UEdGraphPin* PayloadTypePin = FindPinChecked(UK2Node_AsyncAction_RegisterGameEventMessagesName::ActualChannelPinName);
	check(PayloadTypePin->Direction == EGPD_Output)

	return PayloadTypePin;
}
