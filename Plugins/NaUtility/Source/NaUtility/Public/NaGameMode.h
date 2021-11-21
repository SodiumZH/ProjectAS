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
#ifdef NAPACK_MOB_SYSTEM

protected:

	class UNaWorldEnemyControllerManager* EnemyControllerManager;

public: 

	UNaWorldEnemyControllerManager* GetEnemyControllerManager() { return EnemyControllerManager; };

#endif

};



