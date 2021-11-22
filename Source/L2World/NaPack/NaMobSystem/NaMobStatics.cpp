#pragma once

#include "NaMobStatics.h"
#include "NaUtility.h"
#include "NaMob.h"
#include "Combat/NaMobSkill.h"
#include "Combat/NaMobSkillCollision.h"
#include "Combat/NaMobWeapon.h"
#include "Component/NaMobWeaponManager.h"
#include "Component/NaMobPlayerComponent.h"
#include "Component/NaMobSkillManager.h"
#include "Actors/HitDetector.h"

/* Mob */

/* Components */

void UNaMobStatics::GetTimeControl_BP(ANaMob* Target, UTimeControlComponent*& TimeControl) {
	
	if (!IsValid(Target)) {
		TimeControl = nullptr;
		return;
	}

	TimeControl = Target->GetTimeControl();
}

void UNaMobStatics::GetSkillManager_BP(ANaMob* Target, UNaMobSkillManager*& SkillManager) {
	if (!IsValid(Target)) {
		SkillManager = nullptr;
		return;
	}
	SkillManager = Target->GetSkillManager();
}

void UNaMobStatics::GetWeaponManager_BP(ANaMob* Target, UNaMobWeaponManager*& WeaponManager) {
	if (!IsValid(Target)) {
		WeaponManager = nullptr;
		return;
	}
	WeaponManager = Target->GetWeaponManager();
}

void UNaMobStatics::GetBasicStateManager_BP(ANaMob* Target, UNaMobBasicStateManager*& BasicStateManager) {
	if (!IsValid(Target)) {
		BasicStateManager = nullptr;
		return;
	}
	BasicStateManager = Target->GetBasicStateManager();
}

void UNaMobStatics::GetAllRelatives_BP(ANaMob* Target, TArray<AActor*>& RelatedActors) {
	if (!IsValid(Target)) {
		RelatedActors.Empty();
		return;
	}
	Target->GetAllRelatives(RelatedActors);
}

/* Skill & Weapon possessions */

void UNaMobStatics::GetAllSkillNames_BP(ANaMob* Target, TArray<FName>& SkillNames) {
	if (!IsValid(Target)) {
		SkillNames.Empty();
		return;
	}
	SkillNames = Target->GetSkillManager()->GetAllRegisterNames();
}

void UNaMobStatics::GetAllSkills_BP(ANaMob* Target, TArray<ANaMobSkill*>& Skills) {
	if (!IsValid(Target)) {
		Skills.Empty();
		return;
	}
	Skills = Target->GetSkillManager()->GetAllSkills();
}

void UNaMobStatics::GetAllWeaponNames_BP(ANaMob* Target, TArray<FName>& WeaponNames) {
	if (!IsValid(Target)) {
		WeaponNames.Empty();
		return;
	}
	WeaponNames = Target->GetWeaponManager()->GetAllRegisterNames();
}

void UNaMobStatics::GetAllWeapons_BP(ANaMob* Target, TArray<ANaMobWeapon*>& Weapons) {
	if (!IsValid(Target)) {
		Weapons.Empty();
		return;
	}
	Weapons = Target->GetWeaponManager()->GetAllWeapons();
}


/* Type */

bool UNaMobStatics::IsPlayerMob_BP(ANaMob* Target) {
	if (!IsValid(Target))
		return false;
	return Target->IsPlayerMob();
}

UNaMobPlayerComponent* UNaMobStatics::GetPlayerComponent_BP(ANaMob* Target) {
	if (!IsValid(Target))
		return false;
	return Target->GetPlayerComponent();
}

bool UNaMobStatics::IsEnemyMob_BP(ANaMob* Target) {
	if (!IsValid(Target))
		return false;
	return Target->IsEnemyMob();
}

UNaMobEnemyComponent* UNaMobStatics::GetEnemyComponent_BP(ANaMob* Target) {
	if (!IsValid(Target))
		return false;
	return Target->GetEnemyComponent();
}


/* Anim Switch */
void UNaMobStatics::GetAnimStateSwitch_BP(ANaMob* Target, FName Key, bool& SwitchValue) {

	if (!IsValid(Target)) {
		SwitchValue = false;
		return;
	}

	SwitchValue = Target->GetAnimStateSwitch(Key);
}

void UNaMobStatics::OpenAnimStateSwitch_BP(ANaMob* Target, FName Key, float DeltaTime) {
	
	if (!IsValid(Target)) {
		return;
	}

	Target->OpenAnimStateSwitch(Key, DeltaTime);
}

void UNaMobStatics::CloseAnimStateSwitch_BP(ANaMob* Target, FName Key) {
	if (!IsValid(Target)) {
		return;
	}
	Target->CloseAnimStateSwitch(Key);
}

/* Weapon Registeration */
void UNaMobStatics::GetWeaponRegisterName_BP(ANaMob* Target, ANaMobWeapon* Weapon, FName& RegisterName) {
	if (!IsValid(Target)||!IsValid(Weapon)) {
		RegisterName = TEXT("");
		return;
	}
	RegisterName = Target->GetWeaponManager()->GetRegisterName(Weapon);
}

void UNaMobStatics::GetWeaponFromRegisterName_BP(ANaMob* Target, FName RegisterName, ANaMobWeapon*& Weapon) {
	if (!IsValid(Target)) {
		Weapon = nullptr;
		return;
	}
	Weapon = Target->GetWeaponManager()->GetWeaponFromRegisterName(RegisterName);
}

/* Skill */

void UNaMobStatics::GetTimeControl_BP_Skill(ANaMobSkill* InSkill, UTimeControlComponent*& TimeControl) {
	if (!IsValid(InSkill)) {
		TimeControl = nullptr;
		return;
	}
	TimeControl = InSkill->GetTimeControl();
}

void UNaMobStatics::GetSource_BP_Skill(ANaMobSkill* InSkill, ANaMob*& Source) {
	if (!IsValid(InSkill)) {
		Source = nullptr;
		return;
	}
	Source = InSkill->GetSource(); 
};

void UNaMobStatics::GetSocket_BP_Skill(ANaMobSkill* InSkill, FName& Socket) {
	if (!IsValid(InSkill)) {
		Socket = NAME_None;
		return;
	}
	Socket = InSkill->GetSocket();
};

void UNaMobStatics::GetRegisterName_BP_Skill(ANaMobSkill* InSkill, FName& RegName) {
	if (!IsValid(InSkill)) {
		RegName = NAME_None;
		return;
	}
	RegName = InSkill->GetRegisterName(); 
};

void UNaMobStatics::UseSkillByClass_BP(
	ANaMobSkill*& OutSkill,
	ANaMob* SourceMob,
	TSubclassOf<ANaMobSkill> SkillClass,
	const FTransform & InTransform,
	FName RegisterName,
	const FMobSkillUsageOptions & Options,
	bool ForceSpawn,
	USceneComponent* AttachToComponent,
	FName SocketName
) {
	OutSkill = ANaMobSkill::UseSkillByClass(SourceMob, SkillClass, InTransform, RegisterName, Options, ForceSpawn, AttachToComponent, SocketName);
}

void UNaMobStatics::GetSkillByRegisterName_BP(ANaMob* SourceMob, FName InRegisterName, ANaMobSkill*& Skill) {
	if (!IsValid(SourceMob)) {
		Skill = nullptr;
		return;
	}
	Skill = SourceMob->GetSkillManager()->GetSkillFromRegisterName(InRegisterName);
}

void UNaMobStatics::GetSkillRegisteration_BP(ANaMobSkill* InSkill, ANaMob*& SourceMob, FName& RegisterName) {
	if (!IsValid(InSkill)) {
		SourceMob = nullptr;
		RegisterName = NAME_None;
		return;
	}
	SourceMob = InSkill->GetSource();
	RegisterName = InSkill->GetRegisterName();
}

void UNaMobStatics::GetCollisionSet_BP(ANaMobSkill* Target, TSet<ANaMobSkillCollision*>& CollisionSet) {
	if (!IsValid(Target)) {
		CollisionSet.Empty();
		return;
	}
	CollisionSet = Target->GetCollisionSet_Safe();
}


/* Skill Collision */

void UNaMobStatics::GetSourceSkill_BP(ANaMobSkillCollision* Target, ANaMobSkill*& SourceSkill) {
	if (!IsValid(Target)) {
		SourceSkill = nullptr;
		return;
	}
	SourceSkill = Target->GetSourceSkill(); 
}

void UNaMobStatics::GetSocketName_BP(ANaMobSkillCollision* Target, FName& SocketName) {
	if (!IsValid(Target)) {
		SocketName = NAME_None;
		return;
	}
	SocketName = Target->GetSocketName(); 
}

void UNaMobStatics::GetDetector_BP(ANaMobSkillCollision* Target, AHitDetectorInterface*& Detector) {
	if (!IsValid(Target)) {
		Detector = nullptr;
		return;
	}
	Detector = Target->GetDetector();
}

void UNaMobStatics::MakeCollisionByClass_BP(
	ANaMobSkillCollision*& OutCollision,
	ANaMobSkill* SourceSkill,
	TSubclassOf<ANaMobSkillCollision> Class,
	const FTransform & InTranform,
	USceneComponent* AttachToComponent,
	FName Socket,
	bool DoAttachment
) {
	OutCollision = ANaMobSkillCollision::MakeCollisionByClass(SourceSkill, Class, InTranform, AttachToComponent, Socket, DoAttachment);
}

void UNaMobStatics::GetOwnerMob_BP_Weapon(ANaMobWeapon* InWeapon, ANaMob*& OwnerMob) {
	if (!IsValid(InWeapon)) {
		OwnerMob = nullptr;
		return;
	}
	OwnerMob = InWeapon->GetOwnerMob();
}

void UNaMobStatics::HasOwner_BP_Weapon(ANaMobWeapon* InWeapon, bool& HasOwner) {
	if (!IsValid(InWeapon)) {
		HasOwner = false;
		return;
	}
	HasOwner = InWeapon->HasOwner();
}

void UNaMobStatics::GetRegName_BP_Weapon(ANaMobWeapon* InWeapon, FName& RegName) {
	if (!IsValid(InWeapon)) {
		RegName = NAME_None;
		return;
	}
	RegName = InWeapon->GetRegisterName();
}

void UNaMobStatics::GetSocketName_BP_Weapon(ANaMobWeapon* InWeapon, FName& SocketName) {
	if (!IsValid(InWeapon)) {
		SocketName = NAME_None;
		return;
	}
	SocketName = InWeapon->GetSocketName();
}

