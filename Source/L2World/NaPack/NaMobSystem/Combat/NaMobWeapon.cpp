#pragma once

#include "NaMobWeapon.h"
#include "../NaMob.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "../../NaUtility/NaUtility.h"

ANaMobWeapon::ANaMobWeapon() {

	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
}

bool ANaMobWeapon::HaveOwner() {
	return IsValid(OwnerMob);
}

void ANaMobWeapon::CheckOwner(bool TryFixing) {
	
#if WITH_EDITOR

	/* Case of wrong ownership corresponding to simulation */
	if (HaveOwner() && Mesh->IsSimulatingPhysics()) {
		LogError("The weapon has an owner but simulating physics.");
		if (TryFixing)
			OwnerMob = nullptr;
	}

	if (!HaveOwner() && !Mesh->IsSimulatingPhysics()) {
		LogError("The weapon is ownerless but not simulating physics.");
		if(TryFixing)
			
	}

#endif
}


ANaMobWeapon* ANaMobWeapon::AddNewWeapon(TSubclassOf<ANaMobWeapon> Class, ANaMob* Target, const FTransform & Transform, FName Socket, bool NoAttachment) {

	if (!IsValid(Target)) {
		LogErrorNoContext("Invalid target. Weapon generation failed. Please note that a valid target mob is still required as world context if enabled no binding.");
		return nullptr;
	}
	
	ANaMobWeapon* Out = static_cast<ANaMobWeapon*>(Target->GetWorld()->SpawnActor(Class.Get(), &InTransform));
	if (NoAttachment) {
		Out->OwnerMob = nullptr;
		Out->SocketName = NAME_None;
		Out->Mesh->SetSimulatePhysics(true);
		Out->Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	}
	else {
		Out->OwnerMob = Target;
		Out->SocketName = Socket;
		Out->AttachToComponent(Target->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Socket);
	}

	return Out;
}
	
	
