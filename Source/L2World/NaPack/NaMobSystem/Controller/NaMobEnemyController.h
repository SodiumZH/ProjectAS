#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NaMobControllerInterface.h"
#include "../../NaGlobalHeader.h"
#include "NaMobEnemyController.generated.h"

class AActor;
class ANaMob;
class UPawnSensingComponent;


UCLASS(Blueprintable)
class NAPACK_API ANaMobEnemyController : public AAIController , public INaMobControllerInterface {
	
	GENERATED_BODY()

public:

	ANaMobEnemyController();

	/* Components */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UPawnSensingComponent* PawnSensing;

	/* Controller interface */

	UFUNCTION()
	virtual AActor* GetTarget() override { return Target; };

	UFUNCTION()
	virtual AActor* SetTarget(AActor* NewTarget) override;

	UFUNCTION(BlueprintNativeEvent, Category = "NaPack|MobSystem|AI")
	void OnTargetReset(AActor* NewTarget);
	void OnTargetReset_Implementation(AActor* NewTarget) {};

	/* AI */
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|AI")
	void SetBehaviorTree(UBehaviorTree* NewTree) {};	// NOT IMPLEMENTED!!!

};