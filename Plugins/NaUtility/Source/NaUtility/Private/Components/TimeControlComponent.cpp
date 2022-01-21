// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TimeControlComponent.h"


FTimePointEvent::FTimePointEvent() {
	TimePointSeconds = 0;
	EventDelegate = FTimePointEventSignature();
}

FTimePointEvent::FTimePointEvent(double Seconds, const FTimePointEventSignature & Event) {
	TimePointSeconds = Seconds;
	EventDelegate = Event;
}

FTimeLoopEvent::FTimeLoopEvent() {
	StartTime = 0;
	LoopTime = 0;
	EventDelegate = FTimeLoopEventSignature();
	NextTime = 0;
}

FTimeLoopEvent::FTimeLoopEvent(double Start, double Loop, const FTimeLoopEventSignature & Event) {
	StartTime = Start;
	LoopTime = Loop;
	EventDelegate = Event;
	NextTime = Start;
}

// Sets default values for this component's properties
UTimeControlComponent::UTimeControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTimeControlComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TimeInit();
	
}


// Called every frame
void UTimeControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Tick_UpdateTime();
	Tick_PointEvents();
	Tick_LoopEvents();
	
}


/* Time */

void UTimeControlComponent::TimeInit() {
	AbsTimeThisTick = AbsTimeLastTick = AbsTimeBeginPlay = FPlatformTime::Seconds();
}

void UTimeControlComponent::Tick_UpdateTime() {
	AbsTimeLastTick = AbsTimeThisTick;
	AbsTimeThisTick = FPlatformTime::Seconds();
}

double UTimeControlComponent::GetTime() {
	return AbsTimeThisTick - AbsTimeBeginPlay;
}

float UTimeControlComponent::GetTimeFloat() {
	return (float)GetTime();
}

double UTimeControlComponent::GetTimeLastTick() {
	return AbsTimeLastTick - AbsTimeBeginPlay;
}

float UTimeControlComponent::GetTimeLastTickFloat() {
	return (float)GetTimeLastTick();
}

float UTimeControlComponent::GetTimeSecondsFromBeginPlay() {
	return (float)GetTime();
}

bool UTimeControlComponent::IsPassing(double TimePoint) {
	return (GetTime() >= TimePoint) && (GetTimeLastTick() < TimePoint || (GetTimeLastTick() == 0 && TimePoint == 0));
}

bool UTimeControlComponent::IsPassing_Float(float TimePoint) {
	return IsPassing((double)TimePoint);
}



void UTimeControlComponent::AddTimePointEvent(FName Name, float TimeSecondsFromNow, const FTimePointEventSignature & Event, bool bForceAdd) {

	if (bForceAdd) {
		PointEvents.Emplace(Name, FTimePointEvent((double)TimeSecondsFromNow + GetTime(), Event));
	}
	else if (!PointEvents.Contains(Name)) {
		PointEvents.Emplace(Name, FTimePointEvent((double)TimeSecondsFromNow + GetTime(), Event));
	}

}

void UTimeControlComponent::AddLoopEvent(FName Name, float TimeSecondsFromNow, float Period, FTimeLoopEventSignature  Event, bool bForceAdd) {
	if (bForceAdd) {
		LoopEvents.Emplace(Name, FTimeLoopEvent((double)TimeSecondsFromNow + GetTime(), (double)Period, Event));
	}
	else if (!PointEvents.Contains(Name)) {
		LoopEvents.Emplace(Name, FTimeLoopEvent((double)TimeSecondsFromNow + GetTime(), (double)Period, Event));
	}
}

void UTimeControlComponent::Tick_PointEvents() {

	
	for (auto & Elem : PointEvents) {
		if (IsPassing(Elem.Value.TimePointSeconds)) {
			Elem.Value.EventDelegate.ExecuteIfBound();
			PointEventsToBeRemoved.Add(Elem.Key);
			PointEventsToBeRemoved.Add(Elem.Key);
		}
	}
	if (PointEventsToBeRemoved.Num() > 0) {
		for (auto & RemvElem : PointEventsToBeRemoved) {
			PointEvents.Remove(RemvElem);
		}
		PointEventsToBeRemoved.Empty();
	}

}

void UTimeControlComponent::Tick_LoopEvents() {

	for (auto & Elem : LoopEvents) {
		if (IsPassing(Elem.Value.NextTime)) {
			if (Elem.Value.EventDelegate.IsBound()) {
				Elem.Value.EventDelegate.Execute((float)GetTime());
			}
			Elem.Value.NextTime += Elem.Value.LoopTime;
		}
	}

}

void UTimeControlComponent::RemoveTimePointEvent(FName Name, bool ExecuteBeforeRemove) {

	FTimePointEvent* EventToRemove = PointEvents.Find(Name);
	if (EventToRemove) {
		if(ExecuteBeforeRemove)
			EventToRemove->EventDelegate.ExecuteIfBound();
		PointEvents.Remove(Name);
	}

}

void UTimeControlComponent::RemoveLoopEvent(FName Name, bool ExecuteBeforeRemove) {

	FTimeLoopEvent* EventToRemove = LoopEvents.Find(Name);
	if (EventToRemove) {
		if (ExecuteBeforeRemove && EventToRemove->EventDelegate.IsBound())
			EventToRemove->EventDelegate.Execute((float)GetTime());
		LoopEvents.Remove(Name);
		EventToRemove = nullptr;
	}

}

