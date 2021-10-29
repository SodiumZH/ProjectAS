#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../NaGlobalHeader.h"
#include "NaObjectStatics.generated.h"

UCLASS()
class NAPACK_API UNaObjectStatics : public UBlueprintFunctionLibrary {
	
	GENERATED_BODY()

public:

	/* Test if the input object is subject of one of the classes in array. */
	static bool IsSubclassOf(UObject* TestObject, const TArray<UClass*>& ClassArray);


};