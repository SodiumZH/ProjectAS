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

	// Add custom input
	PlayerInputComponent->BindAction("BasicAttack", IE_Pressed, this, &ANaPlayerMob::Action_BasicAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ANaPlayerMob::Action_Dash);
}

/* Controller */

ANaPlayerMobController* ANaPlayerMob::GetPlayerMobController_BP(ANaPlayerMob* PlayerMob) {
	return PlayerMob->GetPlayerMobController();
}

ANaPlayerMobController* ANaPlayerMob::GetPlayerMobController() {
	ANaPlayerMobController* Ctrler = dynamic_cast<ANaPlayerMobController*>(GetController());
	if (Ctrler)
		return Ctrler;
	else LogError("Player mob has no correct controller");
	return nullptr;
}



/* Movement */

void ANaPlayerMob::MoveForward(float val) {

	// Do only when enable movement
	if (!GeneralData.bCanMove)
		val = 0.f;

	if ((Controller != nullptr) && (val != 0.0f))
	{
		AddMovementInput(SpringArm->GetForward(), val);
	}
	ForwardAxisValue = val;
	Tick_PlayerRotation();
}

void ANaPlayerMob::MoveRight(float val) {
	
	// Do only when enable movement
	if (!GeneralData.bCanMove)
		val = 0.f;

	if ((Controller != nullptr) && (val != 0.0f))
	{
		AddMovementInput(SpringArm->GetRight(), val);
	}
	RightAxisValue = val;
}


void ANaPlayerMob::Tick_PlayerRotation() {
	
	// Do only when enable movement
	if (!GeneralData.bCanMove)
		return;

	FVector InputDirection_Local = SpringArm->GetForward()*ForwardAxisValue + SpringArm->GetRight()*RightAxisValue;
	
	// Don't rotate if no input
	if (NaMath::NearlyEqual(InputDirection_Local.Size(), 0.f)) {
		InputDirection = FVector();
		return;
	}

	// Get final input rotation
	InputDirection = InputDirection_Local.GetSafeNormal();
	FRotator InputRot = InputDirection_Local.ToOrientationRotator();

	// Record camera rotation to finally keep the camera static
	FRotator CameraRot = SpringArm->GetRelRot();

	// Get old rotation and delta rotation
	FRotator CurrentRot = GetActorRotation();
	float TargetYaw = NaMath::AngularMoveTo_Deg(CurrentRot.Yaw, InputRot.Yaw, PlayerRotationMaxSpeed);

	// Set rotation and set back the camera
	SetActorRotation(FRotator(CurrentRot.Pitch, TargetYaw, CurrentRot.Roll));
	SpringArm->SetRelRot(CameraRot.Yaw, CameraRot.Pitch);
	
	/*
	UKismetSystemLibrary::PrintString(this,
		TEXT("Input:") + UKismetStringLibrary::Conv_RotatorToString(InputRot), 
		true, false, FLinearColor(0, 0.66, 1),
		1);

	UKismetSystemLibrary::PrintString(this,
		TEXT("Camera:") + UKismetStringLibrary::Conv_RotatorToString(CameraRot),
		true, false, FLinearColor(0, 1, 0.33),
		1);

	UKismetSystemLibrary::PrintString(this,
		TEXT("Actor:") + UKismetStringLibrary::Conv_RotatorToString(GetActorRotation()),
		true, false, FLinearColor(1, 0.33, 0),
		1);

	UKismetSystemLibrary::PrintString(this,
		TEXT("Mesh:") + UKismetStringLibrary::Conv_RotatorToString(GetMesh()->GetComponentRotation()),
		true, false, FLinearColor(0.33, 1, 0),
		1);

*/
}
