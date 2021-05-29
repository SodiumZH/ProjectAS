// Fill out your copyright notice in the Description page of Project Settings.


#include "LMob.h"

// Sets default values
ALMob::ALMob()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALMob::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

