
#include "Components/NaMobEnemyComponent.h"
#include "NaMob.h"
#include "NatriumUtility.h"
#include "WorldManagers/NaWorldEnemyControllerManager.h"
#include "NaPublicDependencies/NaGameModeBaseComponent.h"
#include "NaPublicDependencies/NaPublicDependencyStatics.h"
#include "Utility/DebugUtil/NaDebugUtility.h"

void UNaMobEnemyComponent::BeginPlay() {

	Super::BeginPlay();

	AddController(true);

}

ANaMobEnemyController* UNaMobEnemyComponent::AddController(bool ForceAdd) {
	
	ANaMob* Owner = dynamic_cast<ANaMob*>(GetOwner());
	if (!IsValid(Owner)) {
		LogError("Mob Enemy Component: invalid owner. Owner must be a NaMob.");
		return nullptr;
	}
	else {
		UNaWorldEnemyControllerManager* CtrlMgr = Cast<UNaWorldEnemyControllerManager>(UNaPublicDependencyStatics::GetNaGameModeSubunit(Owner, UNaWorldEnemyControllerManager::StaticClass()));
		if (!IsValid(CtrlMgr)) {
			LogError("Mob Enemy Component: World Enemy Controller Manager on GameMode is invalid.");
			return nullptr;
		}
		return CtrlMgr->AllocateController(Owner, ForceAdd);
		
	}
}