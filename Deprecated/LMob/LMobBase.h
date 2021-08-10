// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LMobData.h"
#include "LMobBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULMobBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class L2WORLD_API ILMobBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/* Race */

	ELMobRace Race = ELMobRace::LRace_None;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "L2W|Mob")
	ELMobRace GetRace();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "L2W|Mob")
	ELMobRace SetRace(ELMobRace NewRace);

	




};
