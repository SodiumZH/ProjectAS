
#include "NaMobEnemyComponent.h"
#include "../NaMob.h"
#include "../../NaUtility/NaUtility.h"
#include "../WorldManager/NaWorldEnemyControllerManager.h"
#include "../../NaGameMode.h"

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
		AGameModeBase* GamemodeBase = Owner->GetWorld()->GetAuthGameMode();
		if (!IsValid(GamemodeBase)) {
			LogError("Mob Enemy Component - Add Controller failed: Game mode isn't valid. Maybe executing on client.");
			return nullptr;
		}
		ANaGameMode* Gamemode = dynamic_cast<ANaGameMode*>(GamemodeBase);
		if (!IsValid(Gamemode)) {
			LogError("Mob Enemy Component - Add Controller failed: To enable NaMobSystem, the game mode must inherit ANaGameMode.");
			return nullptr;
		}
		ANaMobEnemyController* Ctrler = Gamemode->GetEnemyControllerManager()->AllocateController(Owner, ForceAdd);
		return Ctrler;
	}
}