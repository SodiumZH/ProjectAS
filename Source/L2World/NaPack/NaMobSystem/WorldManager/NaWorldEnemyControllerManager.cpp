#pragma once

#include "NaWorldEnemyControllerManager.h"
#include "../Controller/NaMobEnemyController.h"
#include "../NaMob.h"
#include "../../NaGlobalHeader.h"

ANaMobEnemyController* UNaWorldEnemyControllerManager::FindExistingFreeController(TSubclassOf<ANaMobEnemyController> InClass) {

	ANaMobEnemyController* Out = nullptr;
	
	for (int i = 0; i < ActiveControllers.Num();++i) {
		if (ActiveControllers[i]->GetClass() == InClass.Get() && (!IsValid(ActiveControllers[i]->GetPawn()))) {
			Out = ActiveControllers[i];
			break;
		}
	}

	return Out;
}

ANaMobEnemyController* UNaWorldEnemyControllerManager::GetFreeController(TSubclassOf<ANaMobEnemyController> InClass) {

	ANaMobEnemyController* Out = FindExistingFreeController(InClass);
	if (Out)
		return Out;

	Out = static_cast<ANaMobEnemyController*>(GetOwner()->GetWorld()->SpawnActor(InClass.Get()));
	ActiveControllers.Emplace(Out);
	return Out;

}

ANaMobEnemyController* UNaWorldEnemyControllerManager::AllocateController(ANaMob* InMob, bool ForcePossess) {

	if (!IsValid(InMob)) {
		LogWarning("Allocate controller: Input mob is invalid. Return null.");
		return nullptr;
	}
	if (!InMob->IsEnemyMob()) {
		LogWarning("Allocate controller: Input mob doesn't contain an enemy component. Return null.")
		return nullptr;
	}

	ANaMobEnemyController* Out = nullptr; 
	if (!IsValid(InMob->Controller)) {
		Out = GetFreeController(InMob->GetEnemyComponent()->ControllerClass);
		Out->Possess(InMob);
		return Out;
	}
	else if (ForcePossess) {
		InMob->Controller->UnPossess();
		Out = GetFreeController(InMob->GetEnemyComponent()->ControllerClass);
		Out->Possess(InMob);
		return Out;
	}
	else {
		LogWarning("Allocate controller failed: input mob is possessed. Return null. If need to possess anyway, set ForcePossess = true.");
		return nullptr;
	}
	checkNoEntry();
}
