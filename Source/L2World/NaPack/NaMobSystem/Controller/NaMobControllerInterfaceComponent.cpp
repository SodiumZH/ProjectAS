#include "NaMobControllerInterface.h"
#include "NaMobControllerInterfaceComponent.h"

ANaMobPlayerController::ANaMobPlayerController() {
	
	PrimaryActorTick.bCanEverTick = true;

	ControllerInterface = CreateDefaultSubobject<UNaMobControllerInterfaceComponent>(TEXT("ControllerInterface"));


}