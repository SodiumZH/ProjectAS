#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../NaGlobalHeader.h"
#include "../../NaUtility/NaUtility.h"
#include "NaMobControllerInterface.generated.h"

DECLARE_DELEGATE_OneParam(FMobTargetResetSignature, AActor*);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNaMobControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class NAPACK_API INaMobControllerInterface
{
	GENERATED_BODY()

protected:

	AActor* Target = nullptr;


public:

	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Controller")
	virtual AActor* GetTarget();

	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Controller")
	virtual AActor* SetTarget(AActor* NewTarget);

	UFUNCTION(BlueprintNativeEvent, Category = "NaPack|MobSystem|Controller")
	void OnTargetReset(AActor* NewActor);
	virtual void OnTargetReset_Implementation(AActor* NewActor) {};
};