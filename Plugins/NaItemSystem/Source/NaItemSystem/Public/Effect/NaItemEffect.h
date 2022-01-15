#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaItemEffect.generated.h"


UCLASS()
class NAITEMSYSTEM_API ANaItemEffect :public AActor {

	GENERATED_BODY()

public:

	AActor* Source;
	AActor* Target;

};