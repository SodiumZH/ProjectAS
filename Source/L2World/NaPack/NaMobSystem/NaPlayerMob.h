// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NaMob.h"
#include "../NaComponent/FreeLookSpringArmComponent.h"
#include "../NaGlobalHeader.h"
#ifndef _NA_PLAYER_MOB_
#include "NaPlayerMobController.h"
#define _NA_PLAYER_MOB_
#endif
#include "NaPlayerMob.generated.h"

/**
 NaPlayerMob is the Mob class for players. The controller should always be NaPlayerMobController, or it will not work correctly. 
 */

/* Custom Actions */
#define DECLARE_CUSTOM_ACTION(ActionName) \
	UFUNCTION(BlueprintImplementableEvent, Category = "NaPack|MobSystem|PlayerMob|CustomPlayerActions")\
	virtual void Action_##ActionName##(){};

#define DECLARE_CUSTOM_AXIS(AxisName) \
	UFUNCTION(BlueprintImplementableEvent, Category = "NaPack|MobSystem|PlayerMob|CustomPlayerActions")\
	virtual void Axis_##AxisName##(float val){};

UCLASS(BlueprintType)
class NAPACK_API ANaPlayerMob : public ANaMob
{
	GENERATED_BODY()

public:

	ANaPlayerMob();
	virtual void Tick(float dt) override;
	


	/* Input Component*/

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/* Controller related */

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPlayerMobController"), Category = "NaPack|MobSystem|PlayerMob")
	ANaPlayerMobController* GetPlayerMobController_BP(ANaPlayerMob* PlayerMob);
	ANaPlayerMobController* GetPlayerMobController();
	
protected:
	
	/* Camera Input */
	class UFreeLookSpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	SetupFreeLookInput_Header(SpringArm);

protected:


	/* Movement module */
	void MoveForward(float val);
	void MoveRight(float val);

	// Record axis value. These 2 values are for calculating the input orientation.
	float ForwardAxisValue = 0.f;
	float RightAxisValue = 0.f;
	// Actual input direction (zero or normalized).
	FVector InputDirection = FVector();

	// Align player rotation to the input orientation tickly
	void Tick_PlayerRotation();

public:

	// Actual input direction (zero or normalized).
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem|PlayerMob")
	FVector GetInputDirection() { return InputDirection; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMob|Movement")
	float PlayerRotationMaxSpeed = 4.5;

	/* Custom Inputs */

protected:
	DECLARE_CUSTOM_ACTION(BasicAttack)
	DECLARE_CUSTOM_ACTION(Dash)

};
