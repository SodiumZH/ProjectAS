// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LMob.h"
#include "../Components/FreeLookSpringArmComponent.h"
#include "NaMobSystem_Config.h"
#include "LPlayerMob.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ALPlayerMob : public ALMob
{
	GENERATED_BODY()

public:

	ALPlayerMob();

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	class UFreeLookSpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	DECLARE_FREE_LOOK_INPUT(SpringArm)

private:

	void MoveForward(float val);
	void MoveRight(float val);


};
