// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FreeLookSpringArmComponent.h"
#include "FreeLookSpringArmComponent.generated.h"

/**
 * Free-look spring arm is a spring arm that can receive rotating, pitching and zooming input.
 * Designed for character control in third-person game. 
 */
UCLASS()
class L2WORLD_API UFreeLookSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Displ"))



};
