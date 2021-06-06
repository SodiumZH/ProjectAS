// Fill out your copyright notice in the Description page of Project Settings.


#include "LPlayerMob.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"

ALPlayerMob::ALPlayerMob() {

	SpringArm = CreateDefaultSubobject<UFreeLookSpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); 
	Camera->bUsePawnControlRotation = false;

}

void ALPlayerMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupFreeLookInput_Default(ALPlayerMob, SpringArm, PlayerInputComponent);

}