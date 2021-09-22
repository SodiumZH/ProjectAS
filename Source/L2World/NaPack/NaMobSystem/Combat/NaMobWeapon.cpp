#pragma once

#include "NaMobWeapon.h"
#include "../NaMob.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "../../NaUtility/NaUtility.h"

ANaMobWeapon::ANaMobWeapon() {

	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

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
		TArray<USceneComponent*> AllComponents;
		Out->Mesh->SetSimulatePhysics(true);
		
		}

	}
	
	
}