#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../NaGlobalHeader.h"
#include "NaMobPlayerController.generated.h"


UCLASS(Blueprintable)
class NAPACK_API ANaMobPlayerController :public APlayerController {

	GENERATED_BODY()

public:

	ANaMobPlayerController();

	virtual void OnConstruction(const FTransform & Transform) override;

//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Components */

	/* Public interface component of player and enemy controller. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNaMobControllerInterfaceComponent* ControllerInterface;

	/* Interface component to Na UI System. Any interaction with UI system should be handled in this component. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNaControllerUIInterfaceComponent* UIInterface;



	/* Mouse actions */

	/* Switch if show default mouse cursor. 
	This doesn't affect the UI Mouse Cursor (In-game mouse cursor). 
	@ReturnValue value of bShowMouseCursor after switching.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Control|Mouse")
	void SwitchShowMouseCursor();



	/* Selecting actors */

	void SelectActor();

	/* Classes of actors(including subclasses) which can be selected with mouse. If not set, the mouse will be unable to select anything.
	When determining if an actor can be selected, it will read filter first, then exclusions, lastly inclusions. 
	That is, "Excluded" is for excluding some classes in filter list, and "Included" in for removing some classes from "Excluded" list.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassFilter;

	/* Classes of actors(including subclasses) that will be excluded by mouse selection. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassExcluded;

	/* Classes of actors(excluding subclasses) that will be excluded by mouse selection. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassExcludedSpecific;

	/* Classes of actors(including subclasses) that will be included by mouse selection, ignoring if it is listed in exclusions. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassIncluded;

	/* Classes of actors(excluding subclasses) that will be included by mouse selection, ignoring if it is listed in exclusions. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassIncludedSpecific;
	
	// Check if an actor can be selected
	// NOT IMPLEMENTED
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Control|Selection")
	bool CanSelect(AActor* InActor) { return true; };

	/* If true, the filter will add NaMob by default. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MobPlayerController|Selection")
	bool bDefaultCanSelectMob = true;

};