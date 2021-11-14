#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NaMobControllerInterface.h"
#include "../../NaGlobalHeader.h"
#include "NaMobPlayerController.generated.h"

class UNaMobControllerInterfaceComponent;

UCLASS(Blueprintable)
class NAPACK_API ANaMobPlayerController :public APlayerController {

	GENERATED_BODY()

public:

	ANaMobPlayerController();

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Components */

	UFUNCTION(BlueprintReadOnly, EditAnywhere)
	UNaMobControllerInterfaceComponent* ControllerInterface;

	/* Mouse actions */

	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Control|Mouse")
	void SwitchShowMouseCursor();

	UFUNCTION(BlueprintNativeEvent, Category = "NaPack|MobSystem|Control|Mouse")
	void OnSwitchShowMouseCursor();
	virtual void OnSwitchShowMouseCursor_Implementation() {};


	/* Mouse pointing actors */
	/* This module */


	// Trace the actor at which the mouse cursor is pointing
	void Tick_TraceMousePointed();

	// Max distance the actor at which the mouse cursor can point
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Mouse")
	float MaxPointDistance = 10000.0;

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
};