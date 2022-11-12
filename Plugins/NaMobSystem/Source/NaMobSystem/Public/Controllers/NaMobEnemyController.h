#pragma once

#include "CoreMinimal.h"
#include "Controllers/NaMobNonplayerControllerBase.h"
#include "NaMobEnemyController.generated.h"

class AActor;
class ANaMob;
class UPawnSensingComponent;


UCLASS(Blueprintable)
class ANaMobEnemyController : public UNaMobNonplayerControllerBase {
	
	GENERATED_BODY()

public:

	ANaMobEnemyController();

	/* Components */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNaMobControllerInterfaceComponent* ControllerInterface;
	
	/* Controller interface */

	

	/* AI */
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|AI")
	void SetBehaviorTree(UBehaviorTree* NewBehaviorTree) {};	// NOT IMPLEMENTED!!!

};