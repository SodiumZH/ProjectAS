// Copyright Epic Games, Inc. All Rights Reserved.

#include "L2WorldGameMode.h"
#include "L2WorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AL2WorldGameMode::AL2WorldGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
