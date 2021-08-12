#pragma once

#include "CoreMinimal.h"
#include "../NaGlobalHeader.h"
#include "NaMobData.generated.h"


// Mob gender 
UENUM(BlueprintType)
enum class EMobGender:uint8 {
	Gender_None	UMETA(DisplayName = "None"),
	Gender_Male	UMETA(DisplayName = "Male"),
	Gender_Female	UMETA(DisplayName = "Female"),
	Gender_Other	UMETA(DisplayName = "Other"),
	Gender_Custom	UMETA(DisplayName = "Custom Gender")
};

USTRUCT(BlueprintType)
struct FNaMobBasicInformation {

	GENERATED_USTRUCT_BODY()

public:

	FNaMobBasicInformation() {};

	// Mob's display name in UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = TEXT("Mob");

	// Mob's gender
	// If not needed, just ignore it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMobGender Gender = EMobGender::Gender_None;

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

	// Current health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInvincible = false;

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

/*
*	Mob's constant variables. This should not be changed during gameplay.
*/
USTRUCT(BlueprintType)
struct FNaMobConstants {

	GENERATED_USTRUCT_BODY()

public:

	FNaMobConstants() {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BasicWalkSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BasicAccel = 2048.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Basic Jump Z Velocity")
	float BasicJumpZVelocity = 420.f;


	
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