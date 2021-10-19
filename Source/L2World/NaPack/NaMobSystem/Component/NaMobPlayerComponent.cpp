// Fill out your copyright notice in the Description page of Project Settings.


#include "NaMobPlayerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../NaUtility/NaUtility.h"
#include "NaMobStateManager.h"

// Sets default values for this component's properties
UNaMobPlayerComponent::UNaMobPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNaMobPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNaMobPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


/* Basic */

ANaMob* UNaMobPlayerComponent::GetMob() {
	ANaMob* res = dynamic_cast<ANaMob*>(GetOwner());
	if (res) 
		return res;
	else LogError("Mob Player Component attached to a non-mob actor.");
	return nullptr;
}

/* Camera */


/* Input */

void UNaMobPlayerComponent::SetupMobPlayerInput(UInputComponent* PlayerInputComponent) {
	
	check(PlayerInputComponent); 
	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &UFreeLookSpringArmComponent::InputZoomIn);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &UFreeLookSpringArmComponent::InputZoomOut);
	PlayerInputComponent->BindAxis("CameraMouseRotate", this, &UFreeLookSpringArmComponent::InputRotation_M);
	PlayerInputComponent->BindAxis("CameraMousePitch", this, &UFreeLookSpringArmComponent::InputPitch_M);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &UNaMobPlayerComponent::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &UNaMobPlayerComponent::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &UNaMobPlayerComponent::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &UNaMobPlayerComponent::MoveRight);

}

void UNaMobPlayerComponent::MoveForward(float val) {

	ANaMob* Mob = GetMob();
	if (!Mob) 
		return;

	// Do only when enable movement
	if (!Mob->GetStateManager()->CanMove())
		val = 0.f;

	if ((Mob->Controller != nullptr) && (val != 0.0f))
	{
		Mob->AddMovementInput(GetForward(), val);
	}
	ForwardAxisValue = val;
	Tick_PlayerRotation();
}

void UNaMobPlayerComponent::MoveRight(float val) {

	ANaMob* Mob = GetMob();
	if (!Mob)
		return;

	// Do only when enable movement
	if (!Mob->GetStateManager()->CanMove())
		val = 0.f;

	if ((Mob->Controller != nullptr) && (val != 0.0f))
	{
		Mob->AddMovementInput(GetRight(), val);
	}

	RightAxisValue = val;
}

void UNaMobPlayerComponent::Tick_PlayerRotation() {

	ANaMob* Mob = GetMob();
	if (!Mob)
		return;
	// Do only when enable movement
	if (!Mob->GetStateManager()->CanMove())
		return;
	// When falling no rotation
	if (Mob->GetCharacterMovement()->IsFalling())
		return;


	FVector InputDirection_Local = GetForward()*ForwardAxisValue + GetRight()*RightAxisValue;

	// Don't rotate if no input
	if (NaMath::NearlyEqual(InputDirection_Local.Size(), 0.f)) {
		InputDirection = FVector();
		return;
	}

	// Get final input rotation
	InputDirection = InputDirection_Local.GetSafeNormal();
	FRotator InputRot = InputDirection_Local.ToOrientationRotator();

	// Record camera rotation to finally keep the camera static
	FRotator CameraRot = GetRelRot();

	// Get old rotation and delta rotation
	FRotator CurrentRot = Mob->GetActorRotation();
	float TargetYaw = NaMath::AngularMoveTo_Deg(CurrentRot.Yaw, InputRot.Yaw, PlayerRotationMaxSpeed);

	// Set rotation and set back the camera
	Mob->SetActorRotation(FRotator(CurrentRot.Pitch, TargetYaw, CurrentRot.Roll));
	SetRelRot(CameraRot.Yaw, CameraRot.Pitch);

}

void UNaMobPlayerComponent::Jump() {
	ANaMob* Mob = GetMob();
	if (Mob)
		Mob->Jump();
}
void UNaMobPlayerComponent::StopJumping() {
	ANaMob* Mob = GetMob();
	if (Mob)
		Mob->StopJumping();
}

