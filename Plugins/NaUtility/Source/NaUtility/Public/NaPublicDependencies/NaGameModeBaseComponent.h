// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/SceneComponent.h"
#include "NaGameModeBaseComponent.generated.h"


/**
* NaGameModeBaseComponent is a container component for GameMode as all NaPack GameMode dependencies.
* This component should be attached only to the root component of GameMode.
* All other NaPack GameMode components should be directly attached to this component.
*/

class UNaGameModeSubunitComponent;

UENUM()
enum class ENaGameModeComponentErrorType : uint8 {
	GMCET_Correct	UMETA(DisplayName = "Correct"),	// Everything is right
	GMCET_NoBase	UMETA(DisplayName = "Base Is Not Loaded"),	// GameModeBase is not loaded
	GMCET_InvalidGameMode	UMETA(DisplayName = "Invalid Game Mode"),	// Owner actor is not a game mode
	GMCET_BaseDuplicate	UMETA(DisplayName = "Base Duplicated"),	// Base appeared twice
	GMCET_BaseNotAttachedToRoot	UMETA(DisplayName = "Base Not Attached to Root"),	// Base is not attached to root
	GMCET_NonSubunitAttachedToBase	UMETA(DisplayName = "Non-subunit Attached to Base"),		// A non-subunit component is attached to base
	GMCET_SubunitNotAttachedToBase	UMETA(DisplayName = "Subunit Not Attached to Base"),		// A subunit component is not attached to base
	GMCET_SubunitDuplicate	UMETA(DisplayName = "Unduplicatable Subunit Duplicated")	// A subunit disabling duplication appeared twice
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
	//UFUNCTION(BlueprintCallable, Category = "NaPublicDependencies")
	ENaGameModeComponentErrorType CheckGameModeHierarchy(bool bAssertWhenFailed = false);


	UNaGameModeSubunitComponent* GetSubunit(TSubclassOf<UNaGameModeSubunitComponent> Class);
	UNaGameModeSubunitComponent* GetSubunitSpecificClass(TSubclassOf<UNaGameModeSubunitComponent> Class);
	void GetAllSubunits(TArray<UNaGameModeSubunitComponent*>& Result, TSubclassOf<UNaGameModeSubunitComponent> Class);
	void GetAllSubunitsSpecificClass(TArray<UNaGameModeSubunitComponent*>& Result, TSubclassOf<UNaGameModeSubunitComponent> Class);



protected:

	void BeginPlay() override;


};

