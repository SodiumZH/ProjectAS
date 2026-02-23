#pragma once

#include "Components/NaMobWeaponManager.h"
#include "NaMob.h"
#include "Combat/NaMobWeapon.h"
#include "Utility/DebugUtil/NaDebugUtility.h"


UNaMobWeaponManager::UNaMobWeaponManager() {

	PrimaryComponentTick.bCanEverTick = false;

}

TArray<ANaMobWeapon*> UNaMobWeaponManager::GetAllWeapons() {
	TArray<ANaMobWeapon*> Out;
	Out.Empty();
	for (auto& Elem : WeaponRegistry) {
		Out.Emplace(Elem.Value);
	}
	return Out;
}

TArray<FName> UNaMobWeaponManager::GetAllRegisterNames() {
	TArray<FName> Out;
	Out.Empty();
	for (auto& Elem : WeaponRegistry) {
		Out.Emplace(Elem.Key);
	}
	return Out;
}

ANaMobWeapon* UNaMobWeaponManager::GetWeaponFromRegisterName(FName Name) {

	if (Name == NAME_None) {
		LogError("Get Weapon From Name Failed: \"None\" is not a valid name.");
		return nullptr;
	}

	ANaMobWeapon** OutPtr = WeaponRegistry.Find(Name);
	if (OutPtr) {	// Check if the name is valid
		if (IsValid(*OutPtr)) {		// Check if the weapon is valid
			if ((*OutPtr)->GetOwnerMob() == dynamic_cast<ANaMob*>(GetOwner())) {		// Check if the weapon is really attached to this mob
				return (*OutPtr);
			}
			else {	// Case when weapon is not attached to this mob
				LogError("Get Weapon From Name: weapon registered to this mob is not correctly attached.");
				return (*OutPtr);	// Return anyway, but print an error msg to log
			}
		}
		else {	// Case when the found weapon is not valid
			LogError("Get Weapon From Name Failed: input name is registered, but the weapon is invalid. Return value will be null.");
			return nullptr;
		}
	}
	else {	// Case when the name is invalid
		LogError("Get Weapon From Name Failed: invalid name. Return value will be null.");
		return nullptr;
	}
}

FName UNaMobWeaponManager::GetRegisterName(ANaMobWeapon* Weapon) {
	TSet<FName> FoundNames;
	for (auto & Reg : WeaponRegistry) {
		if (Reg.Value == Weapon)
			FoundNames.Emplace(Reg.Key);
	}
	if (FoundNames.Num() == 0) {
		LogWarning("Get Register Name: weapon is not found.");
		return NAME_None;
	}
	else if (FoundNames.Num() >= 2) {
		LogError("Get Register Name: multiple register detected on getting weapon register name. This weapon is registered more than once. Will return a random name in the names found.")
	}

	// This is for returning a random element in the found names. If the name is unique, return value will be specific.
	for (auto & FoundName : FoundNames) {
		return FoundName;
	}

	checkNoEntry();
	return NAME_None;
}

void UNaMobWeaponManager::RegisterWeapon(FName Name, ANaMobWeapon* Weapon, bool ReplacementNoWarning) {

	if (Name == NAME_None) {
		LogError("Register Weapon Failed: \"None\" is not a valid name.");
		return;
	}

	ANaMobWeapon** WeaponFoundPtr = WeaponRegistry.Find(Name);
	if (!WeaponFoundPtr) {
		WeaponRegistry.Emplace(Name, Weapon);
		return;
	}
	else {
		if (!ReplacementNoWarning)
			LogWarning("Register Weapon: input name is existing. ");
		WeaponRegistry[Name] = Weapon;
		return;
	}
}

void UNaMobWeaponManager::RemoveWeapon(FName Name) {
	WeaponRegistry.Remove(Name);
}

void UNaMobWeaponManager::RemoveWeapon(ANaMobWeapon* Weapon) {

	FName Key = GetRegisterName(Weapon);
	if (Key == TEXT("")) {
		LogWarning("Remove Weapon: weapon not found.")
	}
	else
		WeaponRegistry.Remove(Key);

}