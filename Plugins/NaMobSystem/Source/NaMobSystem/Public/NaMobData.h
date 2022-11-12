#pragma once

#include "CoreMinimal.h"
#include "NaMobData.generated.h"

#define MOB_BASIC_WALK_SPEED 200.f
#define MOB_BASIC_ACCEL 2048.f
#define MOB_BASIC_JUMP_Z_VELOCITY 420.f 



USTRUCT(BlueprintType)
struct FNaMobBasicInformation {

	GENERATED_USTRUCT_BODY()

public:

	FNaMobBasicInformation() {};

	// Mob's display name in UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = TEXT("Mob");



};

USTRUCT(BlueprintType)
struct FNaMobMovementData {

	GENERATED_USTRUCT_BODY()

public:

	FNaMobMovementData() {};

	// Speed on walking
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeedScale = 1.f;

	// Speed on running
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeedScale = 2.f;

	// Switch of walk/run
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRunning = true;

	// Max acceleration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Acceleration Scale"))
	float MaxAccelScale = 10.f;

	// Max jump height
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeightScale = 1.f;

	// Enable movement?
	bool bCanMove = true;

	// Enable jump?
	bool bCanJump = true;

};

USTRUCT(BlueprintType)
struct FNaMobStamina {

	GENERATED_USTRUCT_BODY()

public:

	// The mob doesn't take damage (and knockback, etc.) when true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInvincible = false;

	// Current health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 CurrentHP = 100;

	// Current magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 CurrentMP = 100;

	// Max health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 MaxHP = 100;

	// Max magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 MaxMP = 100;

	// Whether the mob is dead
	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;

};
/**
* Public state data for all mobs
*/
USTRUCT(BlueprintType)
struct FNaMobGeneralData {

	GENERATED_USTRUCT_BODY()

public: 

	FNaMobGeneralData() {};

	/* Basic Information */

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNaMobBasicInformation BasicInfo;

	/* Movement related */

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNaMobMovementData Movement;

	/* Stamina Related */

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNaMobStamina Stamina;

};


/**
* Data valid only on players
*/
USTRUCT(BlueprintType)
struct FNaMobPlayerData {

	GENERATED_USTRUCT_BODY()

public:

	FNaMobPlayerData() {};

	// Unique ID of player data.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerID = 0;

	
};