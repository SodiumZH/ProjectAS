// Fill out your copyright notice in the Description page of Project Settings.


#include "NaPublicDependencies/NaGameModeSubunitComponent.h"
#include "NaPublicDependencies/NaGameModeBaseComponent.h"

// Sets default values for this component's properties
UNaGameModeSubunitComponent::UNaGameModeSubunitComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNaGameModeSubunitComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNaGameModeSubunitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UNaGameModeBaseComponent* UNaGameModeSubunitComponent::GetBase() {
	UNaGameModeBaseComponent* Res = dynamic_cast<UNaGameModeBaseComponent*>(GetAttachParent());
	checkf((!GetAttachParent())||Res, TEXT("NaGameModeSubunitComponent is not correctly attached to a NaGameModeBaseComponent."));
	return Res;
}


