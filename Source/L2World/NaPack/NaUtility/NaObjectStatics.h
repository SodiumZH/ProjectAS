#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../NaGlobalHeader.h"
#include "NaObjectStatics.generated.h"

class NAPACK_API UNaObjectStatics : public UBlueprintFunctionLibrary {
	
	GENERATED_BODY()

public:

	template <typename T, typename S>
	static bool HaveSubclassOf(const TArray<T> & InArray, S * ClassContextPtr, TArray<S> & CastArray);

//			UFUNCTION(BlueprintPure, Category = "NaPack|Utility")
};