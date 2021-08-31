
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaMob.h"
#include "NaMobStatics.generated.h"


UCLASS()
class NAPACK_API UNaMobStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* World Constants */
	
	// Get world basic walking speed. To set this value, you need to change the macro value in "NaMobData.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static float GetMobBasicWalkSpeed() { return MOB_BASIC_WALK_SPEED; };

	// Get world basic acceleration. To set this value, you need to change the macro value in "NaMobData.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static float GetMobBasicAcceleration() { return MOB_BASIC_ACCEL; };

	// Get world basic jump Z velocity. To set this value, you need to change the macro value in "NaMobData.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static float GetMobBasicJumpZ_Velocity() { return MOB_BASIC_JUMP_Z_VELOCITY; };

	// Get mob default height. To set this value, you need to change the macro value in "NaMob.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static float GetMobDefaultHeight() { return MOB_DEFAULT_HEIGHT; };

	// Get mob default diameter. To set this value, you need to change the macro value in "NaMob.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static float GetMobDefaultDiameter() { return MOB_DEFAULT_DIAMETER; };

};

