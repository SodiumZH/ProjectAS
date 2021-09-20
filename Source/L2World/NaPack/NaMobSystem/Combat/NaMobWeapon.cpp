#pragma once

#include "NaMobWeapon.h"
#include "Components/StaticMeshComponent.h"

ANaMobWeapon::ANaMobWeapon() {

	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

}