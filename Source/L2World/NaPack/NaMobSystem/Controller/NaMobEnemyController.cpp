#include "NaMobEnemyController.h"
#include "Perception/PawnSensingComponent.h"
#include "NaMobControllerInterfaceComponent.h"

ANaMobEnemyController::ANaMobEnemyController() {

	PrimaryActorTick.bCanEverTick = true;

	ControllerInterface = CreateDefaultSubobject<UNaMobControllerInterfaceComponent>(TEXT("ControllerInterface"));
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}