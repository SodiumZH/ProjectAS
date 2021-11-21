#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaSystem.generated.h"


#define ExistingInvalid(ptr) (!IsValid(ptr) && ptr)


UCLASS()
class NAUTILITY_API UNaSystem : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	//static int GetDataSize_BP(int Data);



};
