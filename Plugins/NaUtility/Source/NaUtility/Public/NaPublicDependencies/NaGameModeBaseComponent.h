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

UCLASS(BlueprintType, ClassGroup = (NaPublicDependencies), meta = (BlueprintSpawnableComponent))
class NAUTILITY_API UNaGameModeBaseComponent : public USceneComponent {

	GENERATED_BODY()

public:

	UNaGameModeBaseComponent() {};

	/** 
	* Get NaGameModeBase from gamemode.
	* If there is no NaGameModeBase, return null.
	* ASSERT if duplicated!
	*/
	UFUNCTION(BlueprintPure, Category = "NaPublicDependencies")
	static UNaGameModeBaseComponent* GetNaGameModeBase(AGameModeBase* GameMode);


};

