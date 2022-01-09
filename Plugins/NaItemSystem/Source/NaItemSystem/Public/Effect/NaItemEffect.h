#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemEffect.generated.h"


UCLASS()
class NAITEMSYSTEM_API AItemEffect :public AActor {

	GENERATED_BODY()

public:

	AActor* Source;
	AActor* Target;

};