#pragma once

#include "NaMobSkill.h"
#include "../NaMob.h"
#include "Components/SceneComponent.h"
#include "NaMobSkillCollision.h"
#include "../../NaComponent/TimeControlComponent.h"
#include "../Component/NaMobSkillManager.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
	if (!IsValid(SkillClass)) {
		LogErrorContext("Trying using skill from invalid class. Use skill failed.", SourceMob);
		return nullptr;
	}

	// Check if RegisterName is occupied
	if (!ForceSpawn && SourceMob->GetSkillManager()->ContainsRegisterName(InRegisterName)) {
		LogWarningContext("Use Skill Failed: Register name is occupied. If you need to spawn anyway, set ForceSpawn true.", SourceMob);
		return nullptr;
	}

	/* Check if the mob's state allows to use this skill */

	ANaMobSkill* ClassDefaultObj = dynamic_cast<ANaMobSkill*>(SkillClass->GetDefaultObject());
	if (!IsValid(ClassDefaultObj)) {
		LogErrorContext("Use Skill Failed: Skill class default object is invald.", SourceMob);
		return nullptr;
	}

	if (!ClassDefaultObj->bCanUseOnMoving && !SourceMob->GetVelocity().IsNearlyZero()) {
		LogWriteContext("Use Skill aborted: this skill disables using on moving, and source mob is moving.", SkillClass->GetDefaultObject());
		return nullptr;
	}

	if (!ClassDefaultObj->bCanUseOnJumping && SourceMob->GetCharacterMovement()->IsFalling()) {
		LogWriteContext("Use Skill aborted: this skill disables using on jumping, and source mob is jumping/falling.", SkillClass->GetDefaultObject());
		return nullptr;
	}

	if (!ClassDefaultObj->bCanOverlapOtherSkills && SourceMob->GetSkillManager()->GetAllSkills().Num() >= 1) {
		LogWriteContext("Use Skill aborted: this skill disables using on other skill exists, and source mob has at least one skill.", SkillClass->GetDefaultObject());
		return nullptr;
	}

	if (!ClassDefaultObj->bCanOverlapSubclassedSkills && UNaObjectStatics::ContainsSubclassObject_Templated(SourceMob->GetSkillManager()->GetAllSkills(), SkillClass)) {
		LogWriteContext("Use Skill aborted: this skill disables using on subclassed skill exists, and source mob has at least one subclassed skill of this skill.", SkillClass->GetDefaultObject());
		return nullptr;
	}

	if (!ClassDefaultObj->bCanUseMultipleTimes && UNaObjectStatics::ContainsSpecifiedClassObject_Templated(SourceMob->GetSkillManager()->GetAllSkills(), SkillClass)) {
		LogWriteContext("Use Skill aborted: this skill disables using on subclassed skill exists, and source mob has at least one subclassed skill of this skill.", SkillClass->GetDefaultObject());
		return nullptr;
	}

	/* Set up attachment */

	// If remains null, attach to source
	if (!IsValid(AttachToComponent)) {
		if (AttachToComponent)
			LogWarningContext("Use Skill: Trying attaching skill to invalid component. Will attach to root component of source instead.", SourceMob);
		AttachToComponent = SourceMob->GetRootComponent();
	}

	/* Make final transform */
	FTransform FinalTransform = DoAttachment ? AttachToComponent->GetComponentTransform() * InTransform : InTransform; 


	// Spawn skill
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* ThisWorld = SourceMob->GetWorld();
	AActor* OutSkillActor = ThisWorld->SpawnActor(SkillClass.Get(), &FinalTransform, SpawnParams);
	ANaMobSkill* OutSkill = static_cast<ANaMobSkill*>(OutSkillActor);
	if (!IsValid(OutSkill)) {
		LogErrorContext("Use skill failed: skill actor is not correctly spawned. Maybe it is destroyed on begin play.", SourceMob);
		return nullptr;
	}
	
	// If use world transform, simply return without attachment
	if (!DoAttachment) {
		return OutSkill;
	}

	

	/* Attach */

	OutSkill->Source = SourceMob;
	OutSkill->Socket = SocketName;
	OutSkill->RegisterName = InRegisterName;
	OutSkill->AttachToComponent(AttachToComponent, FAttachmentTransformRules::KeepWorldTransform, SocketName);
	SourceMob->GetSkillManager()->RegisterSkill(InRegisterName, OutSkill);

	if (OutSkill->bLockMovement) {
		SourceMob->GetBasicStateManager()->SetCanMove(false);
	}
	if (OutSkill->bLockJump) {
		SourceMob->GetBasicStateManager()->JumpType = ENaMobJumpType::MJT_NoJump;
	}
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

void ANaMobSkill::AddCollision(ANaMobSkillCollision* InCol) {
	CollisionSet.Emplace(InCol);
}

void ANaMobSkill::RemoveCollision(ANaMobSkillCollision* InCol) {
	CollisionSet.Remove(InCol);
}

void ANaMobSkill::Destroyed() {
	
	/* Destroy all collisions */
	TSet<ANaMobSkillCollision*> DelCols;
	for (auto & Col : CollisionSet) {
		if (IsValid(Col)) {
			DelCols.Emplace(Col);
		}
	}
	for (auto & DelCol : DelCols) {
		if (IsValid(DelCol))
			DelCol->Destroy();
	}

	if (IsValid(Source)) {
	
		/* Unlock movement & jump */
		if (bUnlockMovementOnDestroyed)
			Source->GetBasicStateManager()->SetCanMove(true);
		if (bUnlockJumpWhenDestroyed)
			Source->GetBasicStateManager()->JumpType = ENaMobJumpType::MJT_Default;

		/* Unregister self */
		Source->GetSkillManager()->UnregisterSkill(RegisterName);
	}
	Super::Destroyed();
}


