// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NaMob.h"
#include "../NaComponent/FreeLookSpringArmComponent.h"
#include "../NaGlobalHeader.h"
#include "NaPlayerMob.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NAPACK_API ANaPlayerMob : public ANaMob
{
	GENERATED_BODY()

public:

	ANaPlayerMob();

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float dt) override;
protected:
	
	class UFreeLookSpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	SetupFreeLookInput_Header(SpringArm);

private:


	/* Movement module */
	void MoveForward(float val);
	void MoveRight(float val);

	// Record axis value. These 2 values are for calculating the input orientation.
	float ForwardAxisValue = 0.f;
	float RightAxisValue = 0.f;

	// Align player rotation to the input orientation tickly
	void Tick_PlayerRotation();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMob|Movement")
	float PlayerRotationMaxSpeed = 3.f;
private:
	

};
