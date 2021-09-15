#pragma once

#include "NaMobSkillCollision.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "../NaMob.h"
#include "NaMobSkill.h"
#include "../../NaUtility/NaDebugUtility.h"

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
	
	RootComponent;

}

void ANaMobSkillCollision::BeginPlay() {
	Super::BeginPlay();
}

void ANaMobSkillCollision::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

ANaMobSkillCollision* ANaMobSkillCollision::MakeCollisionByClass(
	ANaMobSkill* InSourceSkill,
	TSubclassOf<ANaMobSkillCollision> Class,
	const FTransform & InTransform,
	USceneComponent* AttachToComponent,
	FName InSocketName,
	float LifeSpan,
	bool DoAttachment
) {
	// If Source is invalid, stop spawning
	if (!IsValid(InSourceSkill)) {
		LogErrorNoContext("Trying generating collision from invalid skill. Generate collision failed.");
		return nullptr;
	}

	// Spawn col
	AActor* OutColActor = InSourceSkill->GetWorld()->SpawnActor(Class.Get(), &InTransform);
	ANaMobSkillCollision* OutCol = static_cast<ANaMobSkillCollision*>(OutColActor);
	check(OutCol);

	if (!DoAttachment) {
		return OutCol;
	}

	/* Set up attachment */

	// If remains null, attach to source
	if (!IsValid(AttachToComponent)) {
		if (AttachToComponent)
			LogWarningContext("Trying attaching skill to invalid component. Will attach to root component of source instead.", InSourceSkill);
		AttachToComponent = InSourceSkill->GetRootComponent();
	}

	/* Attach */

	OutCol->SourceSkill = InSourceSkill;
	OutCol->SocketName = InSocketName;
	OutCol->SetLifeSpan(LifeSpan);

	OutCol->AttachToComponent(AttachToComponent, FAttachmentTransformRules::KeepRelativeTransform, InSocketName);
	return OutCol;
}

void ANaMobSkillCollision::Destroyed() {
	SourceSkill->GetCollisionSet_Unsafe().Remove(this);
	Super::Destroyed();
}

void ANaMobSkillCollision::SendHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	FHitResult& HitResult
){
	// Send only when HitComponent is root collision body
	if (HitComponent->GetOwner() != this)
		return;

	SourceSkill->ReceiveCollisionHit(this, OtherActor, OtherComponent, NormalImpulse, HitResult, true);

}
