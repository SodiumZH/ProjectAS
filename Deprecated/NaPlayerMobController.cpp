// Fill out your copyright notice in the Description page of Project Settings.


#include "NaPlayerMobController.h"


void ANaPlayerMobController::BeginPlay() {
	Super::BeginPlay();
}

ANaPlayerMob* ANaPlayerMobController::GetPlayerMob_BP(ANaPlayerMobController* Controller) {
	return Controller->GetPlayerMob();
}

ANaPlayerMob* ANaPlayerMobController::GetPlayerMob() {
	ANaPlayerMob* Mob = dynamic_cast<ANaPlayerMob*>(GetPawn());
	if (Mob)
		return Mob;
	else LogError("Player mob controller has no correct mob.");
	return nullptr;
}

