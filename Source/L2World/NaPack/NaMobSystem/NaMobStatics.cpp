#pragma once

#include "NaMobStatics.h"
#include "NaMob.h"
#include "Combat/NaMobSkill.h"
#include "Combat/NaMobSkillCollision.h"
#include "Combat/NaMobWeapon.h"

/* Mob */

/* Anim Switch */
bool UNaMobStatics::GetAnimStateSwitch_BP(ANaMob* Target, FName Key, bool& SwitchValue) {
	SwitchValue = Target->GetAnimStateSwitch(Key);
}

void UNaMobStatics::OpenAnimStateSwitch_BP(ANaMob* Target, FName Key, float DeltaTime) {
	Target->OpenAnimStateSwitch(Key, DeltaTime);
}

void UNaMobStatics::CloseAnimStateSwitch_BP(ANaMob* Target, FName Key) {
	Target->CloseAnimStateSwitch(Key);
}

/* Weapon Registeration */
void UNaMobStatics::GetWeaponRegisterName_BP(ANaMob* Target, ANaMobWeapon* Weapon, FName& RegisterName) {
	RegisterName = Target->GetRegisterName(Weapon);
}

void UNaMobStatics::GetWeaponFromRegisterName_BP(ANaMob* Target, FName RegisterName, ANaMobWeapon*& Weapon) {
	Weapon = Target->GetWeaponFromRegisterName(RegisterName);
}

/* Skill */

void UNaMobStatics::GetTimeControl_BP_Skill(ANaMobSkill* Target, UTimeControlComponent*& TimeControl) {
	TimeControl = Target->GetTimeControl();
}

void UNaMobStatics::UseSkillByClass_BP(
	ANaMobSkill*& OutSkill,
	ANaMob* SourceMob,
	TSubclassOf<ANaMobSkill> SkillClass,
	const FTransform & InTransform,
	USceneComponent* AttachToComponent,
	FName SocketName,
	bool DoAttachment
) {
	OutSkill = ANaMobSkill::UseSkillByClass(SourceMob, SkillClass, InTransform, AttachToComponent, SocketName, DoAttachment);
}

void UNaMobStatics::GetCollisionSet_BP(ANaMobSkill* Target, TSet<ANaMobSkillCollision*>& CollisionSet) {
	CollisionSet = Target->GetCollisionSet_Safe();
}


/* Skill Collision */

void UNaMobStatics::GetCollisionRoot_BP(ANaMobSkillCollision* Target, USceneComponent*& CollisionRoot) {
	CollisionRoot = Target->GetCollisionRoot();
}

void UNaMobStatics::GetCollision_BP(ANaMobSkillCollision* Target, UPrimitiveComponent*& Collision) {
	Collision = Target->GetCollision(); 
}

void UNaMobStatics::GetSourceSkill_BP(ANaMobSkillCollision* Target, ANaMobSkill*& SourceSkill) {
	SourceSkill = Target->GetSourceSkill(); 
}

void UNaMobStatics::GetSocketName_BP(ANaMobSkillCollision* Target, FName& SocketName) {
	SocketName = Target->GetSocketName(); 
}

void UNaMobStatics::GetHalfSize_BP(ANaMobSkillCollision* Target, float& X, float& Y, float& Z) {
	FVector V = Target->GetHalfSize();
	X = V.X;
	Y = V.Y;
	Z = V.Z;
}

void UNaMobStatics::GetHalfSize_BP_Vec(ANaMobSkillCollision* Target, FVector& HalfSize) {
	HalfSize = Target->GetHalfSize();
}

void UNaMobStatics::SetHalfSize_BP(ANaMobSkillCollision* Target, float InX, float InY, float InZ) {
	Target->SetHalfSize(InX, InY, InZ);
}

void UNaMobStatics::SetHalfSize_BP_Vec(ANaMobSkillCollision* Target, FVector InVec) {
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

