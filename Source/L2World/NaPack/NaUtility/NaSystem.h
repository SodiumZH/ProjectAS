#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../NaGlobalHeader.h"
#include "NaSystem.generated.h"


#define ExistingInvalid(ptr) (!IsValid(ptr) && ptr)


namespace NaSystem {

	template<typename T>
	int GetDataSize(T Data);

	template<typename T>
	int GetObjectSize(T* ObjPtr);

};


UCLASS()
class NAPACK_API UNaSystem : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	//static int GetDataSize_BP(int Data);
};
