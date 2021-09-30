#pragma once

#include "NaMobSkillCollision.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "../NaMob.h"
#include "NaMobSkill.h"
#include "../../NaUtility/NaDebugUtility.h"


FSkillCollisionHitReturn::FSkillCollisionHitReturn() {
	SourceCollision = nullptr; 
	HitComponent = nullptr;
	OtherActor = nullptr;
	OtherComponent = nullptr;
	NormalImpulse = FVector::ZeroVector;
	HitResult = FHitResult();
}


FSkillCollisionHitReturn::FSkillCollisionHitReturn(
	ANaMobSkillCollision* InSourceCollision,
	UPrimitiveComponent* InHitComponent,
	AActor* InOtherActor,
	UPrimitiveComponent* InOtherComponent,
	FVector InNormalImpulse,
	const FHitResult & InHitResult
) {
	SourceCollision = InSourceCollision;
	HitComponent = InHitComponent;
	OtherActor = InOtherActor;
	OtherComponent = InOtherComponent;
	NormalImpulse = InNormalImpulse;
	HitResult = InHitResult;
}

ANaMobSkillCollision::ANaMobSkillCollision() {

	RootComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("CollisionDefault"));
	

}

void ANaMobSkillCollision::OnConstruction(const FTransform & trans) {

	Super::OnConstruction(trans);

	UPrimitiveComponent* Root = nullptr;

	//RootComponent->DestroyComponent();

	switch (CollisionShape) {
	case ESkillCollisionShape::SCS_Sphere: {
		USphereComponent* ColSph = NewObject<USphereComponent>(this, TEXT("CollisionSphere"));
		ColSph->SetSphereRadius(HalfSizeX);
		Root = static_cast<UPrimitiveComponent*>(ColSph);
		break;
	}
	case ESkillCollisionShape::SCS_Box: {
		UBoxComponent* ColBox = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
		ColBox->SetBoxExtent(FVector(HalfSizeX, HalfSizeY, HalfSizeZ));
		Root = static_cast<UPrimitiveComponent*>(ColBox);
		break;
	}
	case ESkillCollisionShape::SCS_Capsule: {
		UCapsuleComponent* ColCps = NewObject<UCapsuleComponent>(this, TEXT("CollisionCapsule"));
		ColCps->SetCapsuleSize(HalfSizeX, HalfSizeZ);
		Root = static_cast<UPrimitiveComponent*>(ColCps);
		break;
	}
	case ESkillCollisionShape::SCS_StaticMesh: {
		UStaticMeshComponent* ColMsh = NewObject<UStaticMeshComponent>(this, TEXT("CollisionMesh"));
		Root = static_cast<UPrimitiveComponent*>(ColMsh);
		break;
	}
	default: {
		checkNoEntry();
	}
	}
	//Root->RegisterComponent();
	//RootComponent = Root;

	static_cast<UPrimitiveComponent*>(RootComponent)->OnComponentHit.AddDynamic(this, &ANaMobSkillCollision::SendHitDelegateFunc);


}

void ANaMobSkillCollision::SetHalfSize(float InX, float InZ, float InY) {
	switch (CollisionShape) {
	case ESkillCollisionShape::SCS_Sphere: {
		USphereComponent* ColSph = dynamic_cast<USphereComponent*>(RootComponent);
		check(ColSph);
		ColSph->SetSphereRadius(InX);
		break;
	}
	case ESkillCollisionShape::SCS_Box: {
		UBoxComponent* ColBox = dynamic_cast<UBoxComponent*>(RootComponent);
		check(ColBox);
		ColBox->SetBoxExtent(FVector(InX, InY, InZ));
		break;
	}
	case ESkillCollisionShape::SCS_Capsule: {
		UCapsuleComponent* ColCps = dynamic_cast<UCapsuleComponent*>(RootComponent);
		check(ColCps);
		ColCps->SetCapsuleSize(InX, InZ);
		break;
	}
	case ESkillCollisionShape::SCS_StaticMesh: {
		break;
	}
	default: {
		checkNoEntry();
	}
	}
	HalfSizeX = InX;
	HalfSizeY = InY;
	HalfSizeZ = InZ;
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
	if (IsValid(SourceSkill) && SourceSkill->GetCollisionSet_Unsafe().Contains(this)) {
		SourceSkill->GetCollisionSet_Unsafe().Remove(this);
	}
	Super::Destroyed();
}

void ANaMobSkillCollision::SendHit(const FSkillCollisionHitReturn & Data){
	// Send only when HitComponent is root collision body
	if (Data.HitComponent->GetOwner() != this)
		return;

	SourceSkill->ReceiveCollisionHit(Data);

}

void ANaMobSkillCollision::ConstructHit(FSkillCollisionHitReturn & OutHit, UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult & HitResult) {
	OutHit = FSkillCollisionHitReturn(this, HitComponent, OtherActor, OtherComponent, NormalImpulse, HitResult);
}

void ANaMobSkillCollision::SendHitDelegateFunc(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult & HitResult) {
	if (!AlreadyHitActors.Contains(OtherActor)) {
		FSkillCollisionHitReturn OutHit;
		ConstructHit(OutHit, HitComponent, OtherActor, OtherComponent, NormalImpulse, HitResult);
		SendHit(OutHit);
		AlreadyHitActors.Emplace(OtherActor);
	}
}
