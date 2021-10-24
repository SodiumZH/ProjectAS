// Copyright Epic Games, Inc. All Rights Reserved.

#include "NaGameMode.h"

#if NAPACK_MOB_SYSTEM
#include "NaMobSystem/NaMobSystem.h"
#endif

ANaGameMode::ANaGameMode()
{
	
#if NAPACK_MOB_SYSTEM

	DefaultPawnClass = ANaMob::StaticClass();
	EnemyControllerManager = CreateDefaultSubobject<UNaWorldEnemyControllerManager>(TEXT("EnemyControllerManager"));

#endif

}

