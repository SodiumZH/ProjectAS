#pragma once

#include "NaMobSkill.h"

void ANaMobSkill::OnConstruction(const FTransform & Trans) {
	Super::OnConstruction(Trans);
	TimeLastTick = TimeNow = StartTime = GetWorld()->GetTimeSeconds();
}

void ANaMobSkill::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Tick_Timeline();
}

ANaMobSkill* ANaMobSkill::UseSkillByClass(ANaMob* SourceMob, TSubclassOf<ANaMobSkill> SkillClass, const FTransform & InTranform, FName SocketName) {
	
	ANaMobSkill* OutSkill = dynamic_cast<ANaMobSkill*>(SourceMob->GetWorld()->SpawnActor(SkillClass.Get(), &InTransform));
	check(OutSkill);
	OutSkill->Source = SourceMob;
	OutSkill->Socket = SocketName;
	OutSkill->AttachToActor(SourceMob, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	return OutSkill;
}

void ANaMobSkill::AddTimepointEvent(float Time, void(*Event)(void), UObject* Object) {
	check(Object);
	FNaMobSkillTimelineEvent NewEvent;
	NewEvent.BindUObject(Object, Event);
	TimelineMap.Emplace(Time, NewEvent);
}

void ANaMobSkill::AddTimepointEvent(float Time, void(*Event)(void)) {
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