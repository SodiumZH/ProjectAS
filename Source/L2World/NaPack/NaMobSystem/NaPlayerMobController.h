// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../NaUtility/NaUtility.h"
//#ifndef _NA_PLAYER_MOB_CONTROLLER_
#include "NaPlayerMob.h"
//#define _NA_PLAYER_MOB_CONTROLLER_
//#endif
#include "NaPlayerMobController.generated.h"

/**
	NaPlayerMobController is the controller of NaPlayerMob only.
 */
UCLASS(BlueprintType)
class L2WORLD_API ANaPlayerMobController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	ANaPlayerMobController() {};


	
	// Get possessing mob
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPlayerMob"), Category = "NaPack|NaMobSystem")
	static ANaPlayerMob* GetPlayerMob_BP(ANaPlayerMobController* Controller);
	ANaPlayerMob* GetPlayerMob();
	
	/* Custom Actions */
	

};
