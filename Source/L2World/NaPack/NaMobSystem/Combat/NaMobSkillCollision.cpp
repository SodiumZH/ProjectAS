#pragma once

#include "NaMobSkillCollision.h"

ANaMobSkillCollision::ANaMobSkillCollision() {

	RootComponent = CreateDefaultSubobject<UShapeComponent>(TEXT("CollisionDefault"));

}



void ANaMobSkillCollision::OnConstruction(const FTransform & trans) {

	Super::OnConstruction(trans);

	UShapeComponent* temp = nullptr;

	switch (CollisionShape) {
	case ESkillCollisonShape::SCS_Sphere: {
		temp = static_cast<UShapeComponent*>(NewObject<USphereComponent>(this, TEXT("Collision")));
		break;
	}
	case ESkillCollisonShape::SCS_Box: {
		temp = static_cast<UShapeComponent*>(NewObject<UBoxComponent>(this, TEXT("Collision")));
		break;
	}
	case ESkillCollisonShape::SCS_Capsule: {
		temp = static_cast<UShapeComponent*>(NewObject<UCapsuleComponent>(this, TEXT("Collision")));
		break;
	}
	default: {
		checkNoEntry();
	}
	}
	temp->RegisterComponent();
	RootComponent = temp;
	
}

void ANaMobSkillCollision::BeginPlay() {
	Super::BeginPlay();
}

void ANaMobSkillCollision::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}