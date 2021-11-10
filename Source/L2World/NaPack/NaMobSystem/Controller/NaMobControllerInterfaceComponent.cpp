#include "NaMobControllerInterface.h"

AActor* INaMobControllerInterface::GetTarget() {
	return Target;
}

AActor* INaMobControllerInterface::SetTarget(AActor* NewTarget) {
	Target = NewTarget;
	return Target;
}