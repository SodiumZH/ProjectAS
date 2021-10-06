#pragma once

#include "NaMobStatics.h"
#include "../NaUtility/NaUtility.h"
#include "NaMob.h"
#include "Combat/NaMobSkill.h"
#include "Combat/NaMobSkillCollision.h"
#include "Combat/NaMobWeapon.h"
#include "Component/NaMobPlayerComponent.h"
#include "Component/NaMobSkillManager.h"

/* Mob */

/* Components */
bool UNaMobStatics::IsPlayerMob_BP(ANaMob* Target) {

	if (!IsValid(Target)) {
		return false;
	}

#if WITH_EDITOR
	TArray<UNaMobPlayerComponent*> PlayerComps;
	Target->GetComponents<UNaMobPlayerComponent>(PlayerComps);
	if (PlayerComps.Num() >= 2)
		LogErrorContext("Multiple player components detect: only one player component is allowed on a mob.", Target);
	return PlayerComps.Num() > 0;
#else
	UNaMobPlayerComponent* PlayerComp = Target->GetComponentByClass(UNaMobPlayerComponent::StaticClass());
	return PlayerComp;
#endif
}

void UNaMobStatics::GetPlayerComponent_BP(ANaMob* Target, UNaMobPlayerComponent*& PlayerComponent) {

	if (!IsValid(Target)) {
		PlayerComponent = nullptr;
		return;
	}

#if WITH_EDITOR
	TArray<UNaMobPlayerComponent*> PlayerComps;
	Target->GetComponents<UNaMobPlayerComponent>(PlayerComps);
	if (PlayerComps.Num() >= 2)
		LogErrorContext("Multiple player components detect: only one player component is allowed on a mob.", Target);
	PlayerComponent = PlayerComps.Num() > 0 ? PlayerComps[0] : nullptr;
#else
	UNaMobPlayerComponent* PlayerComp = Target->GetComponentByClass(UNaMobPlayerComponent::StaticClass());
	PlayerComponent = PlayerComp ? PlayerComp : nullptr;
#endif

}

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
	RegisterName = Target->GetRegisterName(Weapon);
}

void UNaMobStatics::GetWeaponFromRegisterName_BP(ANaMob* Target, FName RegisterName, ANaMobWeapon*& Weapon) {
	if (!IsValid(Target)) {
		Weapon = nullptr;
		return;
	}
	Weapon = Target->GetWeaponFromRegisterName(RegisterName);
}

/* Skill */

void UNaMobStatics::GetTimeControl_BP_Skill(ANaMobSkill* Target, UTimeControlComponent*& TimeControl) {
	if (!IsValid(Target)) {
		TimeControl = nullptr;
		return;
	}
	TimeControl = Target->GetTimeControl();
}

void UNaMobStatics::UseSkillByClass_BP(
	ANaMobSkill*& OutSkill,
	ANaMob* SourceMob,
	TSubclassOf<ANaMobSkill> SkillClass,
	const FTransform & InTransform,
	FName RegisterName,
	USceneComponent* AttachToComponent,
	FName SocketName,
	bool DoAttachment
) {
	OutSkill = ANaMobSkill::UseSkillByClass(SourceMob, SkillClass, InTransform, RegisterName, AttachToComponent, SocketName, DoAttachment);
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
	SourceMob = InSkill->Source;
	RegisterName = InSkill->RegisterName;
}

void UNaMobStatics::GetCollisionSet_BP(ANaMobSkill* Target, TSet<ANaMobSkillCollision*>& CollisionSet) {
	if (!IsValid(Target)) {
		CollisionSet.Empty();
		return;
	}
	CollisionSet = Target->GetCollisionSet_Safe();
}


/* Skill Collision */

void UNaMobStatics::GetCollisionRoot_BP(ANaMobSkillCollision* Target, USceneComponent*& CollisionRoot) {
	if (!IsValid(Target)) {
		CollisionRoot = nullptr;
		return;
	}
	CollisionRoot = Target->GetCollisionRoot();
}

void UNaMobStatics::GetCollision_BP(ANaMobSkillCollision* Target, UPrimitiveComponent*& Collision) {
	if (!IsValid(Target)) {
		Collision = nullptr;
		return;
	}
	Collision = Target->GetCollision(); 
}

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

void UNaMobStatics::GetHalfSize_BP(ANaMobSkillCollision* Target, float& X, float& Y, float& Z) {
	if (!IsValid(Target)) {
		X = 0; Y = 0; Z = 0;
		return;
	}
	FVector V = Target->GetHalfSize();
	X = V.X;
	Y = V.Y;
	Z = V.Z;
}

void UNaMobStatics::GetHalfSize_BP_Vec(ANaMobSkillCollision* Target, FVector& HalfSize) {
	if (!IsValid(Target)) {
		HalfSize = FVector::ZeroVector;
		return;
	}
	HalfSize = Target->GetHalfSize();
}

void UNaMobStatics::SetHalfSize_BP(ANaMobSkillCollision* Target, float InX, float InY, float InZ) {
	if (!IsValid(Target)) {
		return;
	}
	Target->SetHalfSize(InX, InY, InZ);
}

void UNaMobStatics::SetHalfSize_BP_Vec(ANaMobSkillCollision* Target, FVector InVec) {
	if (!IsValid(Target)) {
		return;
	}
	Target->SetHalfSizeVector(InVec);
}

void UNaMobStatics::MakeCollisionByClass_BP(
	ANaMobSkillCollision*& OutCollision,
	ANaMobSkill* SourceSkill,
	TSubclassOf<ANaMobSkillCollision> Class,
	const FTransform & InTranform,
	USceneComponent* AttachToComponent,
	FName Socket,
	float LifeSpan,
	bool DoAttachment
) {
	OutCollision = ANaMobSkillCollision::MakeCollisionByClass(SourceSkill, Class, InTranform, AttachToComponent, Socket, LifeSpan, DoAttachment);
}

