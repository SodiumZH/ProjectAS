#pragma once

#include "NaMobSkill.h"
#include "../NaMob.h"
#include "Components/SceneComponent.h"
#include "NaMobSkillCollision.h"
#include "../../NaComponent/TimeControlComponent.h"
#include "../Component/NaMobSkillManager.h"

ANaMobSkill::ANaMobSkill(){

	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SkillRoot"));
	TimeControl = CreateDefaultSubobject<UTimeControlComponent>(TEXT("TimeControl"));
}


void ANaMobSkill::OnConstruction(const FTransform & Trans) {
	Super::OnConstruction(Trans);
}

void ANaMobSkill::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ANaMobSkill::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle InitHandle;
	GetWorldTimerManager().SetTimer(InitHandle, this, &ANaMobSkill::Initialized, 0.0001f);
}


UTimeControlComponent* ANaMobSkill::GetTimeControl() {
	return TimeControl;
}

ANaMobSkill* ANaMobSkill::UseSkillByClass(
	ANaMob* SourceMob, 
	TSubclassOf<ANaMobSkill> SkillClass, 
	const FTransform & InTransform, 
	FName InRegisterName,
	bool ForceSpawn,
	USceneComponent* AttachToComponent,
	FName SocketName,
	bool DoAttachment
) {
	// If SourceMob is invalid, stop spawning
	if (!IsValid(SourceMob)) {
		LogErrorNoContext("Trying using skill from invalid mob. Use skill failed.");
		return nullptr;
	}
	
	// Check if RegisterName is occupied
	if (!ForceSpawn && SourceMob->GetSkillManager()->ContainsRegisterName(InRegisterName)) {
		LogWarningContext("Use Skill Failed: Register name is occupied. If you need to spawn anyway, set ForceSpawn true.", SourceMob);
		return nullptr;
	}

	// Spawn skill
	AActor* OutSkillActor = SourceMob->GetWorld()->SpawnActor(SkillClass.Get(), &InTransform);
	ANaMobSkill* OutSkill = static_cast<ANaMobSkill*>(OutSkillActor);
	check(OutSkill);
	
	// If use world transform, simply return without attachment
	if (!DoAttachment) {
		return OutSkill;
	}

	/* Set up attachment */
	
	// If remains null, attach to source
	if (!IsValid(AttachToComponent)) {
		if (AttachToComponent)
			LogWarningContext("Use Skill: Trying attaching skill to invalid component. Will attach to root component of source instead.", SourceMob);
		AttachToComponent = SourceMob->GetRootComponent();
	}

	/* Attach */

	OutSkill->Source = SourceMob;
	OutSkill->Socket = SocketName;
	OutSkill->RegisterName = InRegisterName;
	OutSkill->AttachToComponent(AttachToComponent, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	SourceMob->GetSkillManager()->RegisterSkill(InRegisterName, OutSkill);
	return OutSkill;
}

void ANaMobSkill::ClearCollisionSet() {
	for (auto & Col : CollisionSet) {
		if (!IsValid(Col)) 
			CollisionSet.Remove(Col);
	}
}

TSet<ANaMobSkillCollision*> & ANaMobSkill::GetCollisionSet_Unsafe() {
	return CollisionSet;
}

TSet<ANaMobSkillCollision*> & ANaMobSkill::GetCollisionSet_Safe() {
	ClearCollisionSet();
	return CollisionSet;
}

void ANaMobSkill::Destroyed() {
	for (auto & Col : CollisionSet) {
		if (IsValid(Col)) {
			Col->Destroy();
		}
	}
	if (IsValid(Source))
		Source->GetSkillManager()->UnregisterSkill(RegisterName);
	Super::Destroyed();
}


