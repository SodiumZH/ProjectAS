// Fill out your copyright notice in the Description page of Project Settings.


#include "NaPlayerMob.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"

ANaPlayerMob::ANaPlayerMob() {

	// Camera and control
	SpringArm = CreateDefaultSubobject<UFreeLookSpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); 
	Camera->bUsePawnControlRotation = false;

	// Controller affect camera only
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ANaPlayerMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupFreeLookInput_Default(ANaPlayerMob, SpringArm, PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ANaPlayerMob::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANaPlayerMob::MoveRight);


}


void ANaPlayerMob::MoveForward(float val) {
	if ((Controller != nullptr) && (val != 0.0f))
	{
		AddMovementInput(SpringArm->GetForward(), val);
	}
}
void ANaPlayerMob::MoveRight(float val) {
	if ((Controller != nullptr) && (val != 0.0f))
	{
		AddMovementInput(SpringArm->GetRight(), val);
	}
}