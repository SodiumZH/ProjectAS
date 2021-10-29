#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../NaGlobalHeader.h"
#include "../../NaUtility/NaUtility.h"
#include "NaMobWeaponManager.generated.h"

class ANaMob;
class ANaMobWeapon;

UCLASS(ClassGroup = (NaMobSystem), meta = (BlueprintSpawnableComponent))
class NAPACK_API UNaMobWeaponManager : public UActorComponent {

	GENERATED_BODY()

public:

	UNaMobWeaponManager();

protected:

	TMap<FName, ANaMobWeapon*> WeaponRegistry;

public:

	TArray<ANaMobWeapon*> GetAllWeapons();

	ANaMobWeapon* GetWeaponFromRegisterName(FName Name);

	/* Get register name of a weapon. Please note that name "none" is a null return. If empty name is returned, it means the weapon is not registered. */
	FName GetRegisterName(ANaMobWeapon* Weapon);

	// Register a weapon to a mob. Please note that the name "none" cannot be a register name.
	// WARNING: this action will NOT attach the weapon or setup any ownership!!!
	// WARNING: If the same name is existing, this action will replace the old weapon and print warning to log.
	// @Param ReplacementNoWarning If true, warning will not be printed when replacement happens. For cases when replacement is intended.
	void RegisterWeapon(FName Name, ANaMobWeapon* Weapon, bool ReplacementNoWarning = false);

	// Remove a weapon by name. WARNING: this action will NOT detach the weapon!!!
	void RemoveWeapon(FName Name);

	// Remove a weapon by ptr. WARNING: this action will NOT detach the weapon!!!
	// If the weapon is registered more than once, it will remove all.
	void RemoveWeapon(ANaMobWeapon* Weapon);
};