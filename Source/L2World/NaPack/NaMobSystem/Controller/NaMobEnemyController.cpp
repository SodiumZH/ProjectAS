#include "NaMobEnemyController.h"

AActor* ANaMobEnemyController:GetTarget() { return Target; };


AActor* ANaMobEnemyController::SetTarget(AActor* NewTarget) {
	Target = NewTarget;
	return Target;
}