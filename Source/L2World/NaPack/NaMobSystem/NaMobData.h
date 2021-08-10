#pragma once

#include "CoreMinimal.h"
#include "../NaGlobalHeader.h"
#include "NaMobData.generated.h"


/* Mob gender 
* L2 game contains this value, but usage is unknown
*/
UENUM(BlueprintType)
enum class EMobGender:uint8 {
	Gender_None	UMETA(DisplayName = "None"),
	Gender_Male	UMETA(DisplayName = "Male"),
	Gender_Female	UMETA(DisplayName = "Female"),
	Gender_Other	UMETA(DisplayName = "Other"),
	Gender_Custom	UMETA(DisplayName = "Custom Gender")
};

/**
* Public status data for all mobs
*/
USTRUCT(BlueprintType)
struct FNaMobGeneralData {

	GENERATED_USTRUCT_BODY()

public: 

	FNaMobGeneralData() {};

	/* Basic Information */

	// Mob's display name in UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = TEXT("Mob");

	// Mob's gender
	// If not needed, just ignore it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMobGender Gender = EMobGender::Gender_None;



	/* Movement related */

	// Speed on walking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bCanMove = true;

	// Speed on walking
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeedScale = 1.f;

	// Speed on running
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeedScale = 2.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bIsRunning = true;

	// Max acceleration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Acceleration Scale"))
	float MaxAccelScale = 10.f;

	// Enable jump?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bCanJump = true;

	// Max jump height
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeightScale = 1.f;


	/* Stamina Related */

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;


};

/**
* Data valid only on players
*/
USTRUCT(BlueprintType)
struct FNaPlayerStatus {

	GENERATED_USTRUCT_BODY()
public:

	// Unique ID of player data.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerID = 0;

	
};