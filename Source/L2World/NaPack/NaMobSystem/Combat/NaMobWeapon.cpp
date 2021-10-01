#pragma once

#include "NaMobWeapon.h"
#include "../NaMob.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "../NaMob.h"
#include "NaMobSkill.h"
#include "NaMobSkillCollision.h"
#include "../../NaUtility/NaUtility.h"

ANaMobWeapon::ANaMobWeapon() {

	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
}

bool ANaMobWeapon::HasOwner() {
	return IsValid(OwnerMob);
}

void ANaMobWeapon::CheckOwner(bool TryFixing) {
	
#if WITH_EDITOR

	/* Case of wrong ownership corresponding to simulation */
	if (HasOwner() && Mesh->IsSimulatingPhysics()) {
		LogError("The weapon has an owner but simulating physics.");
		if (TryFixing)
			OwnerMob = nullptr;
	}

	if (!HasOwner() && !Mesh->IsSimulatingPhysics()) {
		LogError("The weapon is ownerless but not simulating physics.");
		if (TryFixing) {
			Mesh->SetSimulatePhysics(true);
		}
	}

	if (static_cast<AActor*>(OwnerMob) != GetAttachParentActor()) {
		LogError("The weapon is not attached to its owner.");
		if (TryFixing) {
			if (dynamic_cast<ANaMob*>(GetAttachParentActor())) {
				OwnerMob = dynamic_cast<ANaMob*>(GetAttachParentActor());
			}
		}
	}

	return;

#else

	return;

#endif
}


ANaMobWeapon* ANaMobWeapon::AddNewWeapon(TSubclassOf<ANaMobWeapon> Class, ANaMob* Target, const FTransform & Transform, FName Socket, bool NoAttachment) {

	if (!IsValid(Target)) {
		LogErrorNoContext("Add New Weapon Failed:: invalid target. Weapon generation failed. Please note that a valid target mob is still needed as world context if enabled no attachment.");
		return nullptr;
	}

	if (((Target->GetSkeletalMeshComponents().Num() == 0) || (!IsValid(Target->GetSkeletalMeshComponents()[0]))) && (!NoAttachment)) {
		LogErrorNoContext("Add New Weapon: target doesn't have a valid skeletal mesh. This error emerges if no skeletal mesh is added to the target, or the first skeletal mesh component is invalid. Will not do attachment.");
		NoAttachment = true;
	}
	
	ANaMobWeapon* Out = static_cast<ANaMobWeapon*>(Target->GetWorld()->SpawnActor(Class.Get(), &Transform));
	if (NoAttachment) {
		Out->OwnerMob = nullptr;
		Out->SocketName = NAME_None;
		Out->Mesh->SetSimulatePhysics(true);
		Out->Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	}
	else {
		Out->OwnerMob = Target;
		Out->SocketName = Socket;
		Out->AttachToComponent(Target->GetSkeletalMeshComponents()[0], FAttachmentTransformRules::KeepRelativeTransform, Socket);
	}

	return Out;
}
	
void ANaMobWeapon::GiveWeapon(ANaMobWeapon* Weapon, ANaMob* Target, FName Socket, bool ForceGive) {

	if (!IsValid(Weapon)) {
		LogErrorNoContext("Give Weapon Failed:: invalid weapon. To spawn a new weapon, use \"Add New Weapon\".");
		return;
	}

	if (!IsValid(Target)) {
		LogErrorNoContext("Give Weapon Failed: invalid target mob.");
		return;
	}

	if ((Target->GetSkeletalMeshComponents().Num() == 0) || (!IsValid(Target->GetSkeletalMeshComponents()[0]))) {
		LogErrorNoContext("Give Weapon Failed:: target doesn't have a valid skeletal mesh. This error emerges if no skeletal mesh is added to the target, or the first skeletal mesh component is invalid.");
		return;
	}

	if (ForceGive || (!Weapon->HasOwner())) {
		Weapon->Mesh->SetSimulatePhysics(false);
		Weapon->AttachToComponent(Target->GetSkeletalMeshComponents()[0], FAttachmentTransformRules::KeepRelativeTransform, Socket);
		Weapon->OwnerMob = Target;
		Weapon->SocketName = Socket;
	}
	else if (!ForceGive && Weapon->HasOwner() && (Weapon->GetOwnerMob() != Target)) {
		LogWarningContext("GiveWeapon: trying giving an ownered weapon to another mob without Force Give. Will not do anything.", Target);
	}

	return;

}


void ANaMobWeapon::DropWeapon() {

	if (HasOwner()) {
		Mesh->SetSimulatePhysics(true);
		OwnerMob = nullptr;
		SocketName = NAME_None;
	}
}

ANaMobSkillCollision* ANaMobWeapon::MakeWeaponCollision(ANaMobSkill* SourceSkill, float LifeSpan) {

	if (!IsValid(SourceSkill)) {
		LogError("Make Weapon Collision Failed: invalid source skill.");
		return nullptr;
	}

	if (IsValid(CollisionSpawnInfo.AttachToComponent) && CollisionSpawnInfo.AttachToComponent->GetOwner() != this) {
		LogError("Make Weapon Collision: trying to attach to a component not owned by the weapon. Will attach to root component of weapon instead.");
		CollisionSpawnInfo.AttachToComponent = nullptr;
	}

	return ANaMobSkillCollision::MakeCollisionByClass(
		SourceSkill,
		CollisionSpawnInfo.Class,
		CollisionSpawnInfo.Transform,
		IsValid(CollisionSpawnInfo.AttachToComponent) ? CollisionSpawnInfo.AttachToComponent : RootComponent,
		CollisionSpawnInfo.WeaponSocketName,
		LifeSpan,
		true
	);

}