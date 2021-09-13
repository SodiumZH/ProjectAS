#pragma once

#include "NaMobSkill.h"
#include "../NaMob.h"
#include "Components/SceneComponent.h"
#include "../../NaComponent/TimeControlComponent.h"

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

UTimeControlComponent* ANaMobSkill::GetTimeControl() {
	return TimeControl;
}

ANaMobSkill* ANaMobSkill::UseSkillByClass(ANaMob* SourceMob, TSubclassOf<ANaMobSkill> SkillClass, const FTransform & InTransform, FName SocketName, AActor* AttachToActor, USceneComponent* AttachToComponent) {
	if (!IsValid(SourceMob)) {
		LogErrorNoContext("Trying using skill from invalid mob. Use skill failed.");
		return nullptr;
	}
	
	AActor* OutSkillActor = SourceMob->GetWorld()->SpawnActor(SkillClass.Get(), &InTransform);
	ANaMobSkill* OutSkill = static_cast<ANaMobSkill*>(OutSkillActor);
	check(OutSkill);

	// Set up attachment

	/* Check validity */
	if (!IsValid(AttachToActor)) {
		if (AttachToActor)	// existing but invalid
			LogErrorNoContext("Trying attaching skill to invalid actor. Will attach to source mob instead.");
		AttachToActor = SourceMob;
	}
	if (!IsValid(AttachToComponent)) {
		if (AttachToComponent)
			LogErrorNoContext("Trying attaching skill to invalid component. Will attach to root component instead.");
		AttachToComponent = AttachToActor->GetRootComponent();
	}
	if (AttachToComponent->GetOwner() != AttachToActor) {
		LogErrorNoContext("Trying to attach to a component not owned by the attaching actor. Will attach to root component of the actor of attachment instead.");
		AttachToComponent= AttachToActor->GetRootComponent();
	}

	/* Attach */

	OutSkill->Source = SourceMob;
	OutSkill->Socket = SocketName;

	OutSkill->AttachToComponent(AttachToComponent, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	return OutSkill;
}

