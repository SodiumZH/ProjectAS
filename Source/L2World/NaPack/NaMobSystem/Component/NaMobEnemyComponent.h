#pragma once

#include "CoreMinimal.h"
#include "../../NaGlobalHeader.h"
#include "Components/SceneComponent.h"
#include "NaMobEnemyComponent.generated.h"

/* Mob Enemy Component is a module attaching to mob to work as an enemy. 
* Not compatible with Mob Player Component.
*/

class ANaMob;
class USceneComponent;
class AAIController;


UCLASS(Blueprintable)
class NAPACK_API UNaMobEnemyComponent : public USceneComponent {

	GENERATED_BODY()

public:

	TSubclassOf<AAIController> ControllerClass;

};