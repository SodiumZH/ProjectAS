#pragma once

#include "Components/NaMobStateManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NaMob.h"

UNaMobStateManager::UNaMobStateManager() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UNaMobStateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bDoTickSync)
		StateSync();
}

void UNaMobStateManager::StateSync() {
	OnStateSync();
}


void UNaMobBasicStateManager::BeginPlay() {
	Super::BeginPlay();
	bRunWalkSetting = (MovementType != ENaMobMovementType::MMT_Walk);
}

void UNaMobBasicStateManager::StateSync() {

	ANaMob* Mob = dynamic_cast<ANaMob*>(GetOwner());
	if (!Mob) {
		UE_LOG(LogNaMob, Error, TEXT("Mob Basic Static Manager is only valid for NaMob."));
		return;
	}
	UCharacterMovementComponent* CharMove = Mob->GetCharacterMovement();

	/* Set movement speed */
	switch (MovementType) {
	case ENaMobMovementType::MMT_Run: {
		RealtimeSpeedScale = RunSpeedScale;
		break;
	}
	case ENaMobMovementType::MMT_Walk: {
		RealtimeSpeedScale = WalkSpeedScale;
		break;
	}
	case ENaMobMovementType::MMT_NoMove: {
		RealtimeSpeedScale = 0.f;
		break;
	}
	case ENaMobMovementType::MMT_Special: {
		break;
	}
	default: {
		checkNoEntry();
	}
	}

	/* Set jump height */
	switch (JumpType) {
	case ENaMobJumpType::MJT_Default: {
		RealtimeJumpHeightScale = DefaultJumpHeightScale;
		break;
	}
	case ENaMobJumpType::MJT_NoJump: {
		RealtimeJumpHeightScale = 0.f;
		break;
	}
	case ENaMobJumpType::MJT_Special: {
		break;
	}
	}

	/* Apply values */
	CharMove->MaxWalkSpeed = MOB_BASIC_WALK_SPEED * RealtimeSpeedScale;
	CharMove->MaxAcceleration = MOB_BASIC_ACCEL * MaxAccel;
	CharMove->JumpZVelocity = MOB_BASIC_JUMP_Z_VELOCITY * RealtimeJumpHeightScale;

	Super::StateSync();

}

ENaMobMovementType UNaMobBasicStateManager::SetMovementType(ENaMobMovementType NewType) {

	ENaMobMovementType OldType = MovementType;

	switch (NewType) {
	case ENaMobMovementType::MMT_Run: {
		if (!bRunWalkSetting)
			bRunWalkSetting = true;
		break;
	}
	case ENaMobMovementType::MMT_Walk:{
		if (bRunWalkSetting)
			bRunWalkSetting = false;
		break;
		}
	default: {
		break;
	}
	}

	MovementType = NewType;
	OnSetMovementType(OldType, NewType);
	return NewType;
}

void UNaMobBasicStateManager::SetCanMove(bool CanMove) {
	if (CanMove)
		MovementType = bRunWalkSetting ? ENaMobMovementType::MMT_Run : ENaMobMovementType::MMT_Walk;
	else
		MovementType = ENaMobMovementType::MMT_NoMove;
}

void UNaMobBasicStateManager::SetRunWalk(bool NewRunWalk) {

	bRunWalkSetting = NewRunWalk;

	if (MovementType == ENaMobMovementType::MMT_Run || MovementType == ENaMobMovementType::MMT_Walk) {
		MovementType = NewRunWalk ? ENaMobMovementType::MMT_Run : ENaMobMovementType::MMT_Walk;
	}

	return;

}