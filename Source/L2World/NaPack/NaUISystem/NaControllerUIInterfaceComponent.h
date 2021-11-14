#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../NaGlobalHeader.h"
#include "NaControllerUIInterfaceComponent.generated.h"

UCLASS(Blueprintable)
class NAPACK_API UNaControllerUIInterfaceComponent : public UActorComponent {
	GENERATED_BODY()

public:

	class APlayerController* GetOwnerController();


};