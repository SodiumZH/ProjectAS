#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../NaGlobalHeader.h"
#include "../../NaUtility/NaUtility.h"
#include "NaMobControllerInterfaceComponent.generated.h"


// This class is only for public properties and functions for all mob controllers. 
UCLASS(BlueprintType)
class UNaMobControllerInterfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UNaMobControllerInterfaceComponent();

	// The actor as target of controlled mob.
	UPROPERTY(BlueprintReadWrite)
	class AActor* TargetActor = nullptr;

	


};
