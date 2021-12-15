#include "Utility/BPLibraries/NaObjectStatics.h"
#include "Kismet/KismetMathLibrary.h"

bool UNaObjectStatics::IsSubclassOf_InArray(UObject* TestObject, const TArray<TSubclassOf<UObject>>& ClassArray) {
	TSubclassOf<UObject> InClass = TestObject->GetClass();
	if (!InClass)
		return false;
	for (int i = 0; i < ClassArray.Num(); ++i) {
		if (UKismetMathLibrary::ClassIsChildOf(InClass, ClassArray[i]))
			return true;
	}
	return false;
}

bool UNaObjectStatics::ContainsSubclassObject(const TArray<UObject*> TestObjectArray, TSubclassOf<UObject> InClass) {
	
	if (!InClass)
		return false;

	for (int i = 0; i < TestObjectArray.Num(); ++i) {
		if (UKismetMathLibrary::ClassIsChildOf(TestObjectArray[i]->GetClass(), InClass))
			return true;
	}
	return false;

}

bool UNaObjectStatics::ContainsSpecifiedClassObject(const TArray<UObject*> TestObjectArray, TSubclassOf<UObject> InClass) {
	
	if (!InClass)
		return false;

	for (int i = 0; i < TestObjectArray.Num(); ++i) {
		if (TestObjectArray[i]->GetClass() == InClass)
			return true;
	}
	return false;

}


void UNaObjectStatics::GetAllAttachedComponents(TArray<USceneComponent*>& OutComponents, USceneComponent* Target, TSubclassOf<USceneComponent> FilterClass, bool bIncludeGrandchildren) {
	OutComponents.Empty();
	bool DoFilter = (FilterClass != USceneComponent::StaticClass());
	const TArray<USceneComponent*> & Children = Target->GetAttachChildren();
	if (!Children.Num())
		return;		// Recursion base
	TArray<USceneComponent*> Temp;
	
	// Check class only when do class filter to save resource
	if (DoFilter) {
		for (auto & obj : Children) {
			if (UKismetMathLibrary::ClassIsChildOf(obj->GetClass(), FilterClass))
				OutComponents.Add(obj);		// Get direct children
		}
		if (bIncludeGrandchildren) {
			for (obj : Children) {
				GetAllAttachedComponents(Temp, obj, FilterClass, true);		// Recursively get grandchildren
				OutComponents.Append(Temp);
			}
		}
	}

	else {
		for (auto & obj : Children) {
			// No class check to save resource
			OutComponents.Add(obj);		// Get direct children
		}
		if (bIncludeGrandchildren) {
			for (obj : Children) {
				GetAllAttachedComponents(Temp, obj, USceneComponent::StaticClass(), true);		// Recursively get grandchildren
				OutComponents.Append(Temp);
			}
		}
	}

	return;
}