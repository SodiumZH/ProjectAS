#pragma once

#include "NaMobSkill.h"
#include "../NaMob.h"
#include "Components/SceneComponent.h"

void ANaMobSkill::OnConstruction(const FTransform & Trans) {
	Super::OnConstruction(Trans);
	TimeLastTick = TimeNow = StartTime = GetWorld()->GetTimeSeconds();
}

void ANaMobSkill::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Tick_Timeline();
}

ANaMobSkill* ANaMobSkill::UseSkillByClass(ANaMob* SourceMob, TSubclassOf<ANaMobSkill> SkillClass, const FTransform & InTransform, FName SocketName, USceneComponent* AttachToComponent, AActor* AttachToActor) {
	if (!IsValid(SourceMob)) {
		LogError("Trying using skill from invalid mob. Use skill failed.");
		return nullptr;
	}
	
	AActor* OutSkillActor = SourceMob->GetWorld()->SpawnActor(SkillClass.Get(), &InTransform);
	ANaMobSkill* OutSkill = dynamic_cast<ANaMobSkill*>(OutSkillActor);
	check(OutSkill);

	// Set up attachment

	if (!IsValid(AttachToActor)) {
		if (AttachToActor)
			LogError("Trying attaching skill to invalid actor. Attach to source mob instead.");
		AttachToActor = SourceMob;
	}
	if (!IsValid(AttachToComponent)) {
		if (AttachToComponent)
			LogError("Trying attaching skill to invalid component. Attach to root component instead.");
		AttachToComponent = SourceMob->RootComponent;
	}
	if(AttachToComponent->)

	OutSkill->Source = SourceMob;
	OutSkill->Socket = SocketName;


	OutSkill->AttachToActor(dynamic_cast<AActor*>(SourceMob), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	return OutSkill;
}

void ANaMobSkill::AddTimepointEvent(float Time, void(UObject::*Event)(void), UObject* Object) {
	check(Object);
	FNaMobSkillTimelineEvent NewEvent;
	NewEvent.BindDynamic(Object, Event);
	TimelineMap.Emplace(Time, NewEvent);
}

void ANaMobSkill::AddTimepointEvent(float Time, void(UObject::*Event)(void)) {
	UObject* Object = dynamic_cast<UObject*>(this);
	check(Object);
	AddTimepointEvent(Time, Event, Object);
}

void ANaMobSkill::AddTimepointEvent_Delegate(float Time, FNaMobSkillTimelineEvent Event) {
	TimelineMap.Emplace(Time, Event);
}

float ANaMobSkill::GetTime() {
	check(StartTime > 0);
	check(TimeNow > 0);
	return TimeNow - StartTime;
}

float ANaMobSkill::GetTimeLastTick() {
	check(StartTime > 0);
	check(TimeLastTick > 0);
	return TimeLastTick - StartTime;
}

void ANaMobSkill::Tick_Timeline() {
	// Update time
	TimeLastTick = TimeNow;
	TimeNow = GetWorld()->GetTimeSeconds();

	// Execute event if on time
	if (TimelineMap.Num() > 0) {
		for (auto & Elem : TimelineMap) {
			if ((Elem.Key > GetTimeLastTick()) && (Elem.Key <= GetTime())) {
				Elem.Value.ExecuteIfBound();
			}
		}
	}
}