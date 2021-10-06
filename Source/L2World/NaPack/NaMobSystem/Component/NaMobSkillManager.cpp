#pragma once

#include "NaMobSkillManager.h"
#include "../../NaUtility/NaUtility.h"
#include "../NaMob.h"
#include "../Combat/NaMobSkill.h"


UNaMobSkillManager::UNaMobSkillManager() {

	PrimaryComponentTick.bCanEverTick = false;

}

void UNaMobSkillManager::RegisterSkill(FName RegisterName, ANaMobSkill* InSkill) {

	if (!IsValid(this)) return;

	if (!IsValid(InSkill)) {
		LogError("Register Skill Failed: registering invalid skill.");
		return;
	}

	if (RegisterName == FName(NAME_None)) {
		LogError("Register Skill Failed: \"None\" is an invalid name.");
		return;
	}

	if (SkillRegistry.Contains(RegisterName)) {
		LogError("Register Skill Failed: register name is occupied.");
		return;
	}

	SkillRegistry.Emplace(RegisterName, InSkill);

}

void UNaMobSkillManager::UnregisterSkill(FName RegisterName) {

	if (!IsValid(this)) return;

	if (RegisterName == FName(NAME_None)) {
		LogWarning("Unregister Skill Failed: \"None\" is an invalid name.");
		return;
	}

	if (!SkillRegistry.Remove(RegisterName)) {
		LogWarning("Unregister Skill Failed: register name doesn't exist.");
		return;
	}

	

}

void UNaMobSkillManager::UnregisterSkill(ANaMobSkill* InSkill) {

	if (!IsValid(this)) return;

	bool Succeeded = UNaContainerUtility::RemoveValue<FName, ANaMobSkill*>(SkillRegistry, InSkill);
	if (!Succeeded)
		LogWarning("Unregister Skill Failed: skill doesn't exist.");

}

ANaMobSkill* UNaMobSkillManager::GetSkillFromRegisterName(FName RegisterName) {

	if (!IsValid(this)) return nullptr;
	
	if (RegisterName == FName(NAME_None)) {
		LogWarning("Get Skill Failed: \"None\" is an invalid name.");
	}

	ANaMobSkill** ResPtr = SkillRegistry.Find(RegisterName);
	if (ResPtr)
		return *ResPtr;
	else {
		LogWarning("Get Skill Failed: register name doesn't exist.");
		return nullptr;
	}
}

FName UNaMobSkillManager::GetSkillRegisterName(ANaMobSkill* Skill) {

	if (!IsValid(this)) return NAME_None;

	FName* ResPtr = UNaContainerUtility::FindKeyFromValue(SkillRegistry, Skill);
	if (ResPtr)
		return *ResPtr;
	else {
		LogWarning("Get Register Name Failed: not found.");
		return NAME_None;
	}

}

ANaMobSkill* UNaMobSkillManager::UseSkill(
	TSubclassOf<ANaMobSkill> SkillClass,
	const FTransform & InTransform,
	FName RegisterName,
	class USceneComponent* AttachToComponent,
	FName SocketName,
	bool DoAttachment
) {
	if (!this || !GetOwner())
		return nullptr;
	AActor* OwnerActor = GetOwner();
	ANaMob* OwnerMob = dynamic_cast<ANaMob*>(OwnerActor);
	if (OwnerActor && !OwnerMob) {
		LogError("Use Skill Error: skill manager is attached to a non-mob actor.");
		return nullptr;
	}

	return ANaMobSkill::UseSkillByClass(
		OwnerMob,
		SkillClass,
		InTransform,
		RegisterName,
		AttachToComponent,
		SocketName,
		DoAttachment
	);
}