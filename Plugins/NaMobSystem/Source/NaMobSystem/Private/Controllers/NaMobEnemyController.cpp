#include "Controllers/NaMobEnemyController.h"
#include "Perception/PawnSensingComponent.h"
#include "Controllers/NaMobControllerInterfaceComponent.h"

ANaMobEnemyController::ANaMobEnemyController() {

	PrimaryActorTick.bCanEverTick = true;

	ControllerInterface = CreateDefaultSubobject<UNaMobControllerInterfaceComponent>(TEXT("ControllerInterface"));
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}