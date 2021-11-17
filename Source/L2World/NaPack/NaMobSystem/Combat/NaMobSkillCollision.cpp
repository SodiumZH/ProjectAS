#pragma once

#include "NaMobSkillCollision.h"
#include "Components/ChildActorComponent.h"
#include "../../NaActor/HitDetector.h"
#include "../NaMob.h"
#include "NaMobSkill.h"
#include "../../NaUtility/NaDebugUtility.h"
#include "Kismet/KismetMathLibrary.h"

ANaMobSkillCollision::ANaMobSkillCollision() {

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	HitDetectorSpawner = CreateDefaultSubobject<UChildActorComponent>(TEXT("HitDetector"));
	HitDetectorSpawner->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void ANaMobSkillCollision::OnConstruction(const FTransform & trans) {

	Super::OnConstruction(trans);
	//HitDetector->SetChildActorClass(DetectorClass);

}

void ANaMobSkillCollision::BeginPlay() {

	Super::BeginPlay();
	
	/* Check hit detector child actor class */
	if (!UKismetMathLibrary::ClassIsChildOf(HitDetectorSpawner->GetChildActorClass().Get(), AHitDetectorInterface::StaticClass())) {
		LogError("Mob skill collision config error: hit detector doesn't inherit Hit Detector Interface class. The spawn class of hit detector child actor component should be set a subclass of Hit Detector Interface. This skill collision will be destroyed for safety.");
		if (IsValid(HitDetectorSpawner->GetChildActor()))
			HitDetectorSpawner->GetChildActor()->Destroy();
		Destroy();
		return;
	}

	FTimerHandle InitHandle;
	GetWorldTimerManager().SetTimer(InitHandle, this, &ANaMobSkillCollision::Initialized, 1e-5f);
	GetWorldTimerManager().SetTimer(InitHandle, this, &ANaMobSkillCollision::SetupSendHitDelegate, 1e-5f);
	
	
	

}

void ANaMobSkillCollision::SetupSendHitDelegate() {
	if (GetDetector())
		GetDetector()->OnHitDetected_Delegate.BindDynamic(this, &ANaMobSkillCollision::SendSkillHit);
}

void ANaMobSkillCollision::Destroyed() {
	if (IsValid(SourceSkill) && SourceSkill->GetCollisionSet_Unsafe().Contains(this)) {
		SourceSkill->GetCollisionSet_Unsafe().Remove(this);
	}
	Super::Destroyed();
}

AHitDetectorInterface* ANaMobSkillCollision::GetDetector() {
	return dynamic_cast<AHitDetectorInterface*>(HitDetectorSpawner->GetChildActor());
}


void ANaMobSkillCollision::SendSkillHit(const FHitResult& HitResult) {
	// Check validity
	AActor* HitActor = HitResult.Actor.Get();
	if (!IsValid(SourceSkill) || !IsValid(SourceSkill->GetSource())) {
		GetDetector()->ResumeIgnored(HitActor);	// Should not send hit but has added ignored. Resume it.
		return;
	}

	if (HitActor == static_cast<AActor*>(GetSourceSkill()->GetSource())	// Source Mob
		|| GetSourceSkill()->GetSource()->GetWeaponManager()->GetAllWeapons().Contains(HitActor)	// Weapon of source mob
		)
		return;

	SourceSkill->ReceiveCollisionHit(this, HitResult);

}

ANaMobSkillCollision* ANaMobSkillCollision::MakeCollisionByClass(
	ANaMobSkill* InSourceSkill,
	TSubclassOf<ANaMobSkillCollision> Class,
	const FTransform & InTransform,
	USceneComponent* AttachToComponent,
	FName InSocketName,
	bool DoAttachment
) {
	// If Source is invalid, stop spawning
	if (!IsValid(InSourceSkill)) {
		LogErrorNoContext("Trying generating collision from invalid skill. Generate collision failed.");
		return nullptr;
	}

	/* Set up attachment */

	// If remains null, attach to source
	if (!IsValid(AttachToComponent)) {
		if (AttachToComponent)
			LogWarningContext("Trying attaching skill collision to invalid component. Will attach to root component of source instead.", InSourceSkill);
		AttachToComponent = InSourceSkill->GetRootComponent();
	}

	// Calculate the spawn transform

	// Spawn col
	FTransform Trans = DoAttachment ? (AttachToComponent->GetComponentTransform() * InTransform) : InTransform;
	AActor* OutColActor = InSourceSkill->GetWorld()->SpawnActor(Class.Get(), &Trans);
	ANaMobSkillCollision* OutCol = static_cast<ANaMobSkillCollision*>(OutColActor);
	check(OutCol);

	if (!DoAttachment) {
		return OutCol;
	}



	/* Attach */

	OutCol->SourceSkill = InSourceSkill;
	OutCol->SocketName = InSocketName;
	InSourceSkill->AddCollision(OutCol);

	OutCol->AttachToComponent(AttachToComponent, FAttachmentTransformRules::KeepWorldTransform, InSocketName);
	return OutCol;
}