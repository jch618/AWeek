// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameInput.h"

UAWeekGameInput::UAWeekGameInput()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>
		InputContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Input/IMC_Default.IMC_Default'"));
	mContext = InputContext.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		MoveAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Move.IA_Move'"));
	mMove = MoveAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		RotationAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Look.IA_Look'"));
	mLook = RotationAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		JumpAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Jump.IA_Jump'"));
	mJump = JumpAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		SprintAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Sprint.IA_Sprint'"));
	mSprint = SprintAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		AttackAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Attack.IA_Attack'"));
	mAttack = AttackAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		ZoomAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Zoom.IA_Zoom'"));
	mZoom = ZoomAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		InventoryAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Inventory.IA_Inventory'"));
	mInventory = InventoryAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		MainWidgetAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_MainWidget.IA_MainWidget'"));
	mMainWidget = MainWidgetAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		ChangeWeaponAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Test_ChangeWeapon.IA_Test_ChangeWeapon'"));
	mChangeWeapon = ChangeWeaponAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		InteractAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/Actions/IA_Interact.IA_Interact'"));

	mInteract = InteractAction.Object;

	
}