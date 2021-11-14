#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NaMobControllerInterface.h"
#include "../../NaGlobalHeader.h"
#include "NaMobPlayerController.generated.h"

UCLASS(Blueprintable)
class NAPACK_API ANaMobPlayerController :public APlayerController {

	GENERATED_BODY()

public:

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Input */

	/* Mouse actions */

	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Control|Mouse")
	void SwitchShowMouseCursor();

	UFUNCTION(BlueprintNativeEvent, Category = "NaPack|MobSystem|Control|Mouse")
	void OnSwitchShowMouseCursor();
	virtual void OnSwitchShowMouseCursor_Implementation() {};



};