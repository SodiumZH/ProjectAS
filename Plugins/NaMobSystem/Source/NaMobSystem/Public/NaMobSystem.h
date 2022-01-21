/* Created by Sodium */

/**
	NaMobSystem is a creature framework for third-person RPG, including mobs(creatures) as player, npc, monster, etc.
	Need support of NaUtility, NaActor and NaComponent.
*/

#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "NaMobData.h"
#include "NaMobStatics.h"
#include "Components/NaMobComponent.h"
#include "Combat/NaMobSkill.h"
#include "Combat/NaMobSkillCollision.h"
#include "Combat/NaMobWeapon.h"
#include "Controllers/NaMobEnemyController.h"
#include "Controllers/NaMobPlayerController.h"
#include "WorldManagers/NaWorldEnemyControllerManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FNaMobSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
