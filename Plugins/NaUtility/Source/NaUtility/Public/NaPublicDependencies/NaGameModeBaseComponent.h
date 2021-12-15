// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/SceneComponent.h"
#include "NaGameModeBaseComponent.generated.h"

/* Deprecated */
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

/**
* NaGameModeBaseComponent is a container component for GameMode as all NaPack GameMode dependencies.
* This component should be attached only to the root component of GameMode.
* All other NaPack GameMode components should be directly attached to this component.
*/

class UNaGameModeSubunitComponent;

enum class ENaGameModeComponentErrorType : uint8 {
	GMCET_Correct,	// Everything is right
	GMCET_InvalidGameMode,	// Owner actor is not a game mode
	GMCET_BaseDuplicate,	// Base appeared twice
	GMCET_BaseNotAttachedToRoot,	// Base is not attached to root
	GMCET_NonSubunitAttachedToBase,		// A non-subunit component is attached to base
	GMCET_SubunitNotAttachedToBase,		// A subunit component is not attached to base
	GMCET_SubunitDuplicate	// A subunit disabling duplication appeared twice
};

UCLASS(BlueprintType, ClassGroup = (NaPublicDependencies), meta = (BlueprintSpawnableComponent))
class NAUTILITY_API UNaGameModeBaseComponent : public USceneComponent {

	GENERATED_BODY()

public:

	UNaGameModeBaseComponent() {};

	/* Check if NaGameModeBaseComponent and subunits have correct hierarchy.
	* Correct hierarchy: NaGameModeBaseComponent attached to root; all subunits attach to base component. No other components should be attached to NaGameModeBaseComponent.
	* Also check: Game mode base component not duplicated.
	* @Param bAssertWhenFailed If true, it will CRASH when check failed.
	* @ReturnValue Whether check succeeded.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaPublicDependencies")
	bool CheckGameModeHierarchy(bool bAssertWhenFailed = false);

	/** 
	* Get NaGameModeBase from gamemode.
	* If there is no NaGameModeBase, return null.
	* ASSERT if duplicated!
	*/
	UFUNCTION(BlueprintPure, Category = "NaPublicDependencies")
	static UNaGameModeBaseComponent* GetNaGameModeBase(AGameModeBase* GameMode);


};

