#include "Controllers/NaMobPlayerController.h"
#include "Controllers/NaMobControllerInterfaceComponent.h"
#include "Controllers/NaControllerUIInterfaceComponent.h"

ANaMobPlayerController::ANaMobPlayerController(){

	PrimaryActorTick.bCanEverTick = true;
	ControllerInterface = CreateDefaultSubobject<UNaMobControllerInterfaceComponent>(TEXT("ControllerInterface"));
//	UIInterface = CreateDefaultSubobject<UNaControllerUIInterfaceComponent>(TEXT("UIInterface"));

}


void ANaMobPlayerController::OnConstruction(const FTransform & Transform) {
	Super::OnConstruction(Transform);

}

/*
void ANaMobPlayerController::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SwitchShowMouseCursor", IE_Pressed, this, &ANaMobPlayerController::SwitchShowMouseCursor);
}
*/
void ANaMobPlayerController::SwitchShowMouseCursor() {
	bShowMouseCursor = !bShowMouseCursor;
}