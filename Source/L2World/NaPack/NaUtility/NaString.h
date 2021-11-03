#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../NaGlobalHeader.h"
#include "NaString.generated.h"

namespace LNaString {


};

UCLASS()
class NAPACK_API UNaStringLibrary : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	// Make an unique name from a given raw name and an existing list. It will seem like this: "RawName_0", "RawName_1",...
	UFUNCTION(BlueprintPure, Category = "NaPack|Utility|String")
	static FName MakeUniqueName(FName RawName, const TArray<FName> & ExistingList);

	// Check if a name is derived from a raw name by NaStringLibrary::MakeUniqueName
	// If the end of the name is like "_0", it will be regarded as a derived unique name.
	UFUNCTION(BlueprintPure, Category = "NaPack|Utility|String")
	static bool IsDerivedUniqueName(FName Test, FName RawName);

	// If a name is derived from a certain raw name by NaStringLibrary::MakeUniqueName, get the raw name.
	// Note: If the end of the name is like "_0", it will be regarded as a derived unique name, despite if it's really formed by MakeUniqueName.
	UFUNCTION(BlueprintPure, Category = "NaPack|Utility|String")
	static FName FindRawName(FName Test);

};
