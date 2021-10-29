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
	UFUNCTION(meta = (DisplayName = "Is Subclass Of (Class Array)", Keywords = "child inherit parent"), Category = "NaPack|Utility")
	static bool IsSubclassOf_InArray(UObject* TestObject, const TArray<TSubclassOf<UObject>>& ClassArray);




};