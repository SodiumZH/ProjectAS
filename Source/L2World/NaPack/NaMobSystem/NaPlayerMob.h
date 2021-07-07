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
UCLASS()
class NAPACK_API ANaPlayerMob : public ANaMob
{
	GENERATED_BODY()

public:

	ANaPlayerMob();

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	class UFreeLookSpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	DECLARE_FREE_LOOK_INPUT(SpringArm)

private:

	void MoveForward(float val);
	void MoveRight(float val);


};
