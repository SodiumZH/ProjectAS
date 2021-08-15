// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NaPack/NaMobSystem/NaMob.h"
#include "LMobData.h"
#include "Combat/LCombatRule.h"
#include "LMob.generated.h"

/**
 * 
 */
UCLASS()
class L2WORLD_API ALMob : public ANaMob
{
	GENERATED_BODY()
	
public:
	
	/* Data */

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLMobGeneralData LGeneralData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLMobCombatData LCombatData;





};
