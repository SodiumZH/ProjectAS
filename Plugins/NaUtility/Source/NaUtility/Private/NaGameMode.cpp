// Copyright Epic Games, Inc. All Rights Reserved.

#include "NaGameMode.h"

#ifdef NAPACK_MOB_SYSTEM
#include "NaMobSystem/NaMobSystem.h"
#endif

ANaGameMode::ANaGameMode()
{
	
#ifdef NAPACK_MOB_SYSTEM

	DefaultPawnClass = ANaMob::StaticClass();
	EnemyControllerManager = CreateDefaultSubobject<UNaWorldEnemyControllerManager>(TEXT("EnemyControllerManager"));

#endif

}

