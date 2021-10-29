#include "NaObjectStatics.h"
#include "Kismet/KismetMathLibrary.h"

bool UNaObjectStatics::IsSubclassOf(UObject* TestObject, const TArray<TSubclassOf<UObject>>& ClassArray) {
	TSubclassOf<UObject*> InClass = TestObject->GetClass();
	if (!InClass)
		return false;
	for (int i = 0; i < ClassArray.Num(); ++i) {
		if (UKismetMathLibrary::ClassIsChildOf(InClass, ClassArray[i]))
			return true;
	}
	return false;
}