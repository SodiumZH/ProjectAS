/**
* This is an interface including public functions for all mobs, no matter if it's implemented as pawn or character.
* Mob class includes all "creatures", i.e. those has a controller (AI or player), status (stamina, atk, etc.),...
* E.g. players, monsters, NPCs,...
*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NaMobSystem_Config.h"
#include "LMobData.h"
#include "LMobBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULMobBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAME_PROJECT_API ILMobBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};

