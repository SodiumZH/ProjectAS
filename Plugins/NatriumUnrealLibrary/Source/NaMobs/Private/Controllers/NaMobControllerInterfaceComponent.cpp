#include "Controllers/NaMobControllerInterfaceComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NaMob.h"

UNaMobControllerInterfaceComponent::UNaMobControllerInterfaceComponent() {
	
	PrimaryComponentTick.bCanEverTick = false;


}

void UNaMobControllerInterfaceComponent::BeginPlay() {
	Super::BeginPlay();
	if (bDefaultCanSelectMob) {
		SelectActorClassFilter.AddUnique(ANaMob::StaticClass());
	}
}





bool UNaMobControllerInterfaceComponent::CanSelectClass(TSubclassOf<AActor> InClass) {

	bool res = false;
	
	int i = 0;

	for (i = 0; i < SelectActorClassFilter.Num(); ++i) {
		if (UKismetMathLibrary::ClassIsChildOf(InClass, SelectActorClassFilter[i])) {
			res = true;
			break;
		}
	}

	// If not in filter, false
	if (!res)
		return false;

	// Now input is in filter, check if excluded
	for (i = 0; i < SelectActorClassExcluded.Num(); ++i) {
		if (UKismetMathLibrary::ClassIsChildOf(InClass, SelectActorClassExcluded[i])) {
			res = false;
			break;
		}
	}
	
	// When res is still true (not excluded), check ExcludedSpecific
	if (res) {
		for (i = 0; i < SelectActorClassExcludedSpecific.Num(); ++i) {
			if (InClass == SelectActorClassExcludedSpecific[i]) {
				res = false;
				break;
			}
		}
	}
	
	// If still true, it's not excluded, return true
	if (res)
		return true;

	// Now res is excluded, check if included

	for (i = 0; i < SelectActorClassIncluded.Num(); ++i) {
		if (UKismetMathLibrary::ClassIsChildOf(InClass, SelectActorClassIncluded[i])) {
			res = true;
			break;
		}
	}

	if (!res) {
		for (i = 0; i < SelectActorClassIncludedSpecific.Num(); ++i) {
			if (InClass == SelectActorClassIncludedSpecific[i]) {
				res = true;
				break;
			}
		}
	}

	return res;
}

bool UNaMobControllerInterfaceComponent::CanSelectActor(AActor* InActor) {
	return CanSelectClass(InActor->GetClass());
}

void UNaMobControllerInterfaceComponent::SelectActor(AActor* InActor) {
	
	if (!IsValid(InActor)) {
		ActorSelected = nullptr;
		return;
	}

	else if (CanSelectActor(InActor)) {
		ActorSelected = InActor;
		return;
	}

	return;

}
