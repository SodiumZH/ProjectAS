// By Sodium
#pragma once

#include "CoreMinimal.h"
#include "NaMobItemLibrary.generated.h"

class UNaItemContainerComponent;
class AController;

UCLASS()
class NAMOBSYSTEM_API UNaMobItemLibrary : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	/* If an item container is owned by a mob controller, return the controller. Otherwise return null.
	* @Param PlayerOnly If true, return value only when it's a mob player controller. Otherwise return null, even if it's a mob controller.
	*/
	//UFUNCTION(BlueprintPure, meta = (DisplayName = "GetContainerOwningController", Keywords = "owner"), Category = "NaMobSystem|ItemInterface")
	//void GetContainerOwningController_BP(UNaItemContainerComponent* Target, AController*& AsController);
	//AController* GetContainerOwningController(UNaItemContainerComponent* Target);

	/* If a container is owned by a mob, return the mob. Otherwise return null.
	* @Param PlayerOnly If true, return value only when it's a player mob. Otherwise return null, even if it's a mob.
	*/
	//UFUNCTION(BlueprintPure, meta = (DisplayName = "GetContainerOwningMob", Keywords = "owner"), Category = "NaMobSystem|ItemInterface")
	//void GetContainerOwningMob_BP(UNaItemContainerComponent* Target, ANaMob*& AsMob);
	//ANaMob* GetContainerOwningMob(UNaItemContainerComponent* Target);


};