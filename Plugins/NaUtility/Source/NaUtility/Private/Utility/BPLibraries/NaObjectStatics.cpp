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


