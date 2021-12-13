// Copyright Epic Games, Inc. All Rights Reserved.

#include "NaPublicDependencies/NaGameModeBaseComponent.h"
#include "GameFramework/GameModeBase.h"

ANaGameMode::ANaGameMode()
{
	


}

UNaGameModeBaseComponent* UNaGameModeBaseComponent::GetNaGameModeBase(AGameModeBase* GameMode) {
	TArray<UActorComponent*> Res = GameMode->GetComponentByClass(UNaGameModeBaseComponent::StaticClass());
	checkf(Res.Num() <= 1, TEXT("Error: Duplicated NaGameModeBase component detected."));
	if (Res.Num())
		return Cast<UNaGameModeBaseComponent>(Res[0]);
	else return nullptr;
}