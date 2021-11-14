#include "NaMobPlayerController.h"

void ANaMobPlayerController::OnConstruction(const FTransform & Transform) {
	Super::OnConstruction(Transfrom);

}
void ANaMobPlayerController::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SwitchShowMouseCursor", IE_Pressed, this, &ANaMobPlayerController::SwitchShowMouseCursor);
}

void ANaMobPlayerController::SwitchShowMouseCursor() {
	bShowMouseCursor = !bShowMouseCursor;
	OnSwitchShowMouseCursor();
}