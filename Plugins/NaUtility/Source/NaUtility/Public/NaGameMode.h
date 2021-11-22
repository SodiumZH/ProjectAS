// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NaGameMode.generated.h"

UCLASS()
class NAUTILITY_API ANaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ANaGameMode();

/* Mob system interface */

protected:

	class UNaWorldEnemyControllerManager* EnemyControllerManager = nullptr;

public: 

	UNaWorldEnemyControllerManager* GetEnemyControllerManager() { return EnemyControllerManager; };


};



