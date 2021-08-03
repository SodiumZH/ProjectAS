// Fill out your copyright notice in the Description page of Project Settings.


#include "NaPlayerMob.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
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

void ANaPlayerMob::Tick(float dt) {
	Super::Tick(dt);

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
	ForwardAxisValue = val;
	Tick_PlayerRotation();
}

void ANaPlayerMob::MoveRight(float val) {
	if ((Controller != nullptr) && (val != 0.0f))
	{
		AddMovementInput(SpringArm->GetRight(), val);
	}
	RightAxisValue = val;
}

void ANaPlayerMob::Tick_PlayerRotation() {
	FVector InputOrientation = SpringArm->GetForward()*ForwardAxisValue + SpringArm->GetRight()*RightAxisValue;
	FRotator InputRot = InputOrientation.ToOrientationRotator();


	// Don't rotate if no input
	if (NaMath::NearlyEqual(InputOrientation.Size(), 0.f))
		return;
	
	// Record camera rotation to finally keep the camera static
	FRotator CameraRot = SpringArm->GetRelRot();

	// Get old rotation and delta rotation
	FRotator CurrentRot = GetActorRotation();
	float TargetYaw = InputRot.Yaw;//NaMath::MoveTo(CurrentRot.Yaw, InputRot.Yaw, PlayerRotationMaxSpeed);

	// Set rotation and set back the camera
	SetActorRotation(FRotator(CurrentRot.Pitch, TargetYaw, CurrentRot.Roll));
	//SpringArm->SetRelRot(CameraRot.Yaw, CameraRot.Pitch);
	
	UKismetSystemLibrary::PrintString(this,
		UKismetStringLibrary::Conv_RotatorToString(CurrentRot),
		true, false, FLinearColor(0, 0.66, 1),
		20);

	UKismetSystemLibrary::PrintString(this,
		UKismetStringLibrary::Conv_RotatorToString(CameraRot),
		true, false, FLinearColor(0, 1, 0.33),
		20);

}

