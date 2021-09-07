// Fill out your copyright notice in the Description page of Project Settings.


#include "NaPack/NaComponent/TimeControlComponent.h"


FTimepointEvent::FTimepointEvent(double Seconds, const FTimepointEventSignature & Event) {
	TimePointSeconds = Seconds;
	EventDelegate = Event;
}

FTimeLoopEvent::FTimeLoopEvent(double Start, double Loop, const FTimeLoopEventSignature & Event) {
	StartTime = Start;
	LoopTime = Loop;
	LoopEvent = Event;
	NextTime = StartTime;
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
	
	
}


// Called every frame
void UTimeControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

