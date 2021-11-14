#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NaMobControllerInterface.h"
#include "../../NaGlobalHeader.h"
#include "NaMobPlayerController.generated.h"

class UNaMobControllerInterfaceComponent;

UCLASS(Blueprintable)
class NAPACK_API ANaMobPlayerController :public APlayerController {

	GENERATED_BODY()

public:

	ANaMobPlayerController();

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Components */

	UFUNCTION(BlueprintReadOnly, EditAnywhere)
	UNaMobControllerInterfaceComponent* ControllerInterface;

	/* Mouse actions */

	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Control|Mouse")
	void SwitchShowMouseCursor();

	UFUNCTION(BlueprintNativeEvent, Category = "NaPack|MobSystem|Control|Mouse")
	void OnSwitchShowMouseCursor();
	virtual void OnSwitchShowMouseCursor_Implementation() {};

	/* Mouse pointing actors */

	// Trace the actor at which the mouse cursor is pointing
	void Tick_TraceMousePointed();

	// Max distance the actor at which the mouse cursor can point
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobPlayerController|Mouse")
	float MaxPointDistance = 10000.0;

	void SelectActor();
};