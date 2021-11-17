#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../NaGlobalHeader.h"
#include "NaWorldEnemyControllerManager.generated.h"

/* World manager module for enemy controllers. 
* This module should be attached ONLY on Gamemode for management of enemy AI controllers in the world.
* When an enemy is killed, it will be unpossessed by the controller, producing a "wild" AI controller. 
* This module is intended to generate, allocate and recycle AI controllers so that AI controllers will not be spawned and destroyed too frequently, and prevent accumulation of wild AI controllers.
* It only receive NaMobEnemyController as AI controller class.
*/

class ANaMob;
class ANaMobEnemyController;
class ANaMobPlayerController;

UCLASS(Blueprintable)
class NAPACK_API UNaWorldEnemyControllerManager : public UActorComponent {

	GENERATED_BODY()

public:

	/* Allocation */

	// All enemy controllers in the world.
	UPROPERTY(BlueprintReadOnly)
	TArray<ANaMobEnemyController*> ActiveControllers;

	/* Find a free (not working) enemy controller. If there isn't one, return null. */
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, Keywords = "find get controller"), Category = "NaPack|MobSystem")
	ANaMobEnemyController* FindExistingFreeController(TSubclassOf<ANaMobEnemyController> InClass);

	/* Find a free (not working) enemy controller. If there isn't one, spawn one and return. */
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, Keywords = "find get add controller"), Category = "NaPack|MobSystem")
	ANaMobEnemyController* GetFreeController(TSubclassOf<ANaMobEnemyController> InClass);

	/* Allocate a controller to an enemy mob. If there is an existing free controller, use this controller to possess. Or spawn one and possess.
	* @Param InMob Mob to be possessed. It must contain an enemy component.
	* @Param ForcePossess Whether possess if the input mob has already been possessed.
	* @ReturnValue Allocated controller.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, Keywords = "possess give add"), Category = "NaPack|MobSystem")
	ANaMobEnemyController* AllocateController(ANaMob* InMob, bool ForcePossess = false);

};