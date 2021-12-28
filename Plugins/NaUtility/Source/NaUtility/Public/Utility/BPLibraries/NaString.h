#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "NaString.generated.h"


UCLASS()
class NAUTILITY_API UNaStringLibrary : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	// Make an unique name from a given raw name and an existing list. It will seem like this: "RawName_0", "RawName_1",...
	UFUNCTION(BlueprintPure, Category = "NaUtility|String")
	static FName MakeUniqueName(FName RawName, const TArray<FName> & ExistingList);

	// Check if a name is derived from a raw name by NaStringLibrary::MakeUniqueName
	// If the end of the name is like "_0", it will be regarded as a derived unique name.
	UFUNCTION(BlueprintPure, Category = "NaUtility|String")
	static bool IsDerivedUniqueName(FName Test, FName RawName);

	// If a name is derived from a certain raw name by NaStringLibrary::MakeUniqueName, get the raw name.
	// Note: If the end of the name is like "_0", it will be regarded as a derived unique name, despite if it's really formed by MakeUniqueName.
	UFUNCTION(BlueprintPure, Category = "NaUtility|String")
	static FName FindRawName(FName Test);

	// Transform int to string with given length. 
	// If the string is not long enough, add 0 in the front.
	// If the fixed length is shorter than the length needed, it will be truncated.
	UFUNCTION(BlueprintPure, DisplayName = "Int to String (Fixed Length)", Category = "NaUtility|String")
	static FString Int2StrFixedLen(int InInt, int Len);


};
