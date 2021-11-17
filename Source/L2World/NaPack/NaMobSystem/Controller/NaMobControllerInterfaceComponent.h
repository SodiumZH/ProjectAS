#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../NaGlobalHeader.h"
#include "../../NaUtility/NaUtility.h"
#include "NaMobControllerInterfaceComponent.generated.h"


// This class is only for public properties and functions for all mob controllers. 
UCLASS(BlueprintType)
class UNaMobControllerInterfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UNaMobControllerInterfaceComponent();



	/* Selecting actors */

	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Control|Selection")
		void SelectActor() {};

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
	bool CanSelect(AActor* InActor);

	/* If true, the filter will add NaMob by default. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MobPlayerController|Selection")
	bool bDefaultCanSelectMob = true;
	


};
