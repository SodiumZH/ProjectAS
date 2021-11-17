#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NaMobControllerInterface.h"
#include "../../NaGlobalHeader.h"
#include "NaMobPlayerController.generated.h"


UCLASS(Blueprintable)
class NAPACK_API ANaMobPlayerController :public APlayerController {

	GENERATED_BODY()

public:

	ANaMobPlayerController();

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Components */

	/* Public interface component of player and enemy controller. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNaMobControllerInterfaceComponent* ControllerInterface;

#if NAPACK_UI_SYSTEM

	/* Interface component to Na UI System. Any interaction with UI system should be handled in this component. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNaControllerUIInterfaceComponent* UIInterface;

#endif

	/* Mouse actions */

	/* Switch if show default mouse cursor. 
	This doesn't affect the UI Mouse Cursor (In-game mouse cursor). 
	@ReturnValue value of bShowMouseCursor after switching.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Control|Mouse")
	bool SwitchShowMouseCursor();


<<<<<<< HEAD

	/* Mouse pointing actors */
	/* This module */

=======
	/* Selecting actors */

	void SelectActor();
>>>>>>> 9c940f9911bd2f3dd4575b044f6fe81c791f9dee



<<<<<<< HEAD
	void SelectActor();

	/* Classes of actors(including subclasses) which can be selected with mouse. If not set, the mouse will be unable to select anything.
	When determining if an actor can be selected, it will read filter first, then exclusions, lastly inclusions. 
	That is, "Excluded" is for excluding some classes in filter list, and "Included" in for removing some classes from "Excluded" list.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Mouse|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassFilter;

	/* Classes of actors(including subclasses) that will be excluded by mouse selection. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Mouse|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassExcluded;

	/* Classes of actors(excluding subclasses) that will be excluded by mouse selection. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Mouse|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassExcludedSpecific;

	/* Classes of actors(including subclasses) that will be included by mouse selection, ignoring if it is listed in exclusions. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Mouse|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassIncluded;

	/* Classes of actors(excluding subclasses) that will be included by mouse selection, ignoring if it is listed in exclusions. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Mouse|Selection")
	TArray<TSubclassOf<AActor>> SelectActorClassIncludedSpecific;

	bool CheckCanMouseSelected(AActor* InActor);

	/* If true, the filter will add NaMob by default. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MobPlayerController|Mouse|Selection")
	bool bDefaultCanSelectMob = true;
=======
>>>>>>> 9c940f9911bd2f3dd4575b044f6fe81c791f9dee
};