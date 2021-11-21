// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../NaMob.h"
#include "../NaMobData.h"
#include "Components/FreeLookSpringArmComponent.h"
#include "NaMobPlayerComponent.generated.h"

/*
* NaMobPlayerComponent is an additional module attaching only to NaMob to enable player control.
* This component inherits from FreeLookSpringArmComponent and bring a spring arm. If a camera is needed, bind manually.
* DO NOT attach multiple player components to one mob.
*/

UCLASS( ClassGroup=(NaMobSystem), meta=(BlueprintSpawnableComponent) )
class UNaMobPlayerComponent : public UFreeLookSpringArmComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNaMobPlayerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Basic */
	// Get owning mob
	ANaMob* GetMob();

	/* Free Look Camera */


protected:

	UFreeLookSpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

public:

	virtual void SetupMobPlayerInput(class UInputComponent* PlayerInputComponent);

protected:


	/* Input */
	// Call only in NaMob::SetupPlayerInputComponent()
	//UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem")


	// Movement function bound to input
	void MoveForward(float val);
	void MoveRight(float val);
	void Jump();
	void StopJumping();

	// Record axis value. These 2 values are for calculating the input orientation.
	float ForwardAxisValue = 0.f;
	float RightAxisValue = 0.f;

	// Actual input direction (zero or normalized).
	FVector InputDirection = FVector();

	// Align player rotation to the input orientation tickly
	void Tick_PlayerRotation();

public:

	// Actual input direction (zero or normalized).
	FVector GetInputDirection_Local() { return InputDirection; };
	UFUNCTION(BlueprintPure, DisplayName = "GetInputDirection", Category = "NaPack|MobSystem")
	static FVector GetInputDirection_Static(UNaMobPlayerComponent* Target) { return Target->GetInputDirection_Local(); };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobPlayerComponent|Movement")
	float PlayerRotationMaxSpeed = 9;

	/* Data */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobPlayerComponent|Data")
	FNaMobPlayerData PlayerData;
		
};
