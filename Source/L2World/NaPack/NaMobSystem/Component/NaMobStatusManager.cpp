#pragma once

#include "NaMobStatusManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../NaMob.h"

void UNaMobStatusManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bDoTickSync)
		StatusSync();
}

void UNaMobStatusManager::StatusSync() {

	ANaMob* Mob = dynamic_cast<ANaMob*>(GetOwner());
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

	OnStatusSync();
	
}