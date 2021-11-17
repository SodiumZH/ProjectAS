#include "NaMobControllerInterfaceComponent.h"

UNaMobControllerInterfaceComponent::UNaMobControllerInterfaceComponent() {
	
	PrimaryComponentTick.bCanEverTick = false;


}

bool UNaMobControllerInterfaceComponent::CanSelect(AActor* InActor) {

	bool res = false;
	/*
	for (int i = 0; i < SelectActorClassFilter.Num(); ++i) {
		if(InActor->)
	}
	*/
	return res;
}