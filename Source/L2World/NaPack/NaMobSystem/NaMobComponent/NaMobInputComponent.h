#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Components/ActorComponent.h"
#include "../../NaGlobalHeader.h"
#include "NaMobInputComponent.generated.h"

/**

	This is a common input interface of all mobs, both from AI and player controller.
	S

*/
UCLASS()
class NAPACK_API UNaMobInputComponent : public UActorComponent {

	GENERATED_BODY()

public:

	UNaMobInputComponent();

	// Add a default this component into a mob
	static UNaMobInputComponent* AddDefault(class ANaMob* Parent, FName Name = TEXT("MobInputComponent"));

	/* Actions */

	void MovementInput()


};