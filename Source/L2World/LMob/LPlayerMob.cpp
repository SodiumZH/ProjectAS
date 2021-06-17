// Fill out your copyright notice in the Description page of Project Settings.


#include "LPlayerMob.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"

ALPlayerMob::ALPlayerMob() {

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

void ALPlayerMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupFreeLookInput_Default(ALPlayerMob, SpringArm, PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALPlayerMob::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALPlayerMob::MoveRight);


}


void ALPlayerMob::MoveForward(float val) {
	if ((Controller != nullptr) && (val != 0.0f))
	{
		AddMovementInput(SpringArm->GetForward(), val);
	}
}
void ALPlayerMob::MoveRight(float val) {
	if ((Controller != nullptr) && (val != 0.0f))
	{
		AddMovementInput(SpringArm->GetRight(), val);
	}
}