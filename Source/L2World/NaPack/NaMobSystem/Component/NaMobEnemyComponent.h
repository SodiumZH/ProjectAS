#pragma once

#include "CoreMinimal.h"
#include "../../NaGlobalHeader.h"
#include "Components/SceneComponent.h"
#include "../Controller/NaMobEnemyController.h"
#include "NaMobEnemyComponent.generated.h"


/* Mob Enemy Component is a module attaching to mob to work as an enemy. 
* Not compatible with Mob Player Component.
*/

class ANaMob;
class USceneComponent;



UCLASS(ClassGroup = (NaMobSystem), meta = (BlueprintSpawnableComponent))
class NAPACK_API UNaMobEnemyComponent : public USceneComponent {

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EnemyComponent")
	TSubclassOf<ANaMobEnemyController> ControllerClass = ANaMobEnemyController::StaticClass();

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|NaMobSystem")
	ANaMobEnemyController* AddController(bool ForceAdd = false);

protected:
	
	virtual void BeginPlay() override;




};