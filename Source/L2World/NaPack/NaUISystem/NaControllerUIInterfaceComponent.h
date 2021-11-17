#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../NaGlobalHeader.h"
#include "NaControllerUIInterfaceComponent.generated.h"

UCLASS(Blueprintable)
class NAPACK_API UNaControllerUIInterfaceComponent : public UActorComponent {
	GENERATED_BODY()

public:

	// Set if the UI-implemented mouse cursor(using a custom image) is shown.
	// NOT IMPLEMENTED
	void SetShowUIMouseCursor(bool Value) {};




};