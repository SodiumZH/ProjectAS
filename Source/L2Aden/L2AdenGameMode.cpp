// Copyright Epic Games, Inc. All Rights Reserved.

#include "L2AdenGameMode.h"
#include "L2AdenCharacter.h"
#include "UObject/ConstructorHelpers.h"

AL2AdenGameMode::AL2AdenGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
