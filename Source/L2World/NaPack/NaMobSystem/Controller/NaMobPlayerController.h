#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NaMobPlayerController.generated.h"


UCLASS(Blueprintable)
class ANaMobPlayerController :public APlayerController {

	GENERATED_BODY()

public:

	ANaMobPlayerController();

	virtual void OnConstruction(const FTransform & Transform) override;

//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Components */

	/* Public interface component of player and enemy controller. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNaMobControllerInterfaceComponent* ControllerInterface;

	/* Interface component to Na UI System. Any interaction with UI system should be handled in this component. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNaControllerUIInterfaceComponent* UIInterface;



	/* Mouse actions */

	/* Switch if show default mouse cursor. 
	This doesn't affect the UI Mouse Cursor (In-game mouse cursor). 
	@ReturnValue value of bShowMouseCursor after switching.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|Control|Mouse")
	void SwitchShowMouseCursor();



	

};