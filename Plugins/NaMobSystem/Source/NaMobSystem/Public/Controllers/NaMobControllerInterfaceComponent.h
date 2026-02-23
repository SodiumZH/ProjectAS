#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NatriumUtility.h"
#include "NaMobControllerInterfaceComponent.generated.h"


/* This class is for transferring data from mobs to controllers.
* Including target selection,
*/
UCLASS(BlueprintType)
class UNaMobControllerInterfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UNaMobControllerInterfaceComponent();

	UPROPERTY(BlueprintReadOnly)
	AActor* ActorSelected = nullptr;

	/* Selecting actors */

	/* Select an actor. If the actor is not selectable (defined in class filter), no action. If null is input, it will be set null (i.e. unselect).*/
	UFUNCTION(BlueprintCallable, Category = "NaMobSystem|Control|Selection")
	void SelectActor(AActor* InActor);

	/* Classes of actors(including subclasses) selectable. If not set, the mob will be unable to select anything.
	When determining if an actor can be selected, it will read filter first, then exclusions, lastly inclusions.
	That is, "Excluded" is for excluding some classes in filter list, and "Included" is for removing some classes from "Excluded" list.
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
	UFUNCTION(BlueprintCallable, Category = "NaMobSystem|Control|Selection")
	bool CanSelectActor(AActor* InActor);

	// Check if a class can be selected
	UFUNCTION(BlueprintCallable, Category = "NaMobSystem|Control|Selection")
	bool CanSelectClass(TSubclassOf<AActor> InClass);

	/* If true, the filter will add NaMob by default. */
	UPROPERTY(EditAnywhere, Category = "MobPlayerController|Selection")
	bool bDefaultCanSelectMob = true;
	


};
