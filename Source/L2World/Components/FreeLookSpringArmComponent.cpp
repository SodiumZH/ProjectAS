// Fill out your copyright notice in the Description page of Project Settings.


#include "FreeLookSpringArmComponent.h"

void UFreeLookSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UFreeLookSpringArmComponent::BeginPlay() {
	Super::BeginPlay();
	InitRotateAndZoom();
}

/* Set Rotation */

void UFreeLookSpringArmComponent::ApplyRelRot() {
	this << (InitRot + FObjectRotator(RelRot));
}

void UFreeLookSpringArmComponent::ClampYaw() {
	while (RelRot.Yaw > 180.f) {
		RelRot.Yaw -= 360.f;
		DesiredYaw -= 360.f;
	}
	while (RelRot.Yaw <= -180.f) {
		RelRot.Yaw += 360.f;
		DesiredYaw += 360.f;
	}
}

void UFreeLookSpringArmComponent::SetRelRot(float NewYaw, float NewPitch, bool ForceSet) {
	// Clamp pitch
	NewPitch = SMath::Clamp(NewPitch, -89.9f, 89.9f);
	// Set RelRot first
	RelRot = FRotator(NewPitch, NewYaw, 0.f);	// Never allow roll
	// Apply to object
	ApplyRelRot();
}

void UFreeLookSpringArmComponent::AddRelRot(float DeltaYaw, float DeltaPitch, bool ForceSet) {
	float NewPitch = RelRot.Pitch + DeltaPitch;
	float NewYaw = RelRot.Yaw + DeltaYaw;
	SetRelRot(NewYaw, NewPitch, ForceSet);
}

void UFreeLookSpringArmComponent::ResetInitRot(FRotator NewInitialRotation, bool bKeepVertical) {
	if (bKeepVertical)
		NewInitialRotation = FRotator(NewInitialRotation.Pitch, 0.f, 0.f);
	InitRot = NewInitialRotation; // Rotation from old InitRot to new
	ApplyRelRot();
}


/* Input Functions */
void UFreeLookSpringArmComponent::InputRotation_M(float val) {
	DesiredYaw += (val*YawSens_M);
}
void UFreeLookSpringArmComponent::InputPitch_M(float val) {
	DesiredPitch += (val*PitchSens_M);
	DesiredPitch = SMath::Clamp(DesiredPitch, -89.9f, 89.9f);
}
void UFreeLookSpringArmComponent::InputRotation_K(float val) {
	DesiredYaw += (val*YawSens_K);
}
void UFreeLookSpringArmComponent::InputPitch_K(float val) {
	DesiredPitch += (val*PitchSens_K);
	DesiredPitch = SMath::Clamp(DesiredPitch, -89.9f, 89.9f);
}
void UFreeLookSpringArmComponent::InputZoomIn() {
	if(bReverseZoom)
		DesiredLength += ZoomSens;
	else DesiredLength -= ZoomSens;
	// Clamp length
	DesiredLength = SMath::Clamp(DesiredLength, MinLength, MaxLength);
}
void UFreeLookSpringArmComponent::InputZoomOut() {
	if (bReverseZoom)
		DesiredLength -= ZoomSens;
	else DesiredLength += ZoomSens;
	// Clamp length
	DesiredLength = SMath::Clamp(DesiredLength, MinLength, MaxLength);
}

/* Tickly-applied Rotation/Zoom */
void UFreeLookSpringArmComponent::TickRotateDirect() {
	// Skip if no need to rotate
	if (SMath::NearlyEqual(DesiredPitch + DesiredYaw, RelRot.Pitch + RelRot.Yaw, SMALL_NUMBER))
		return;
	
	RelRot = FRotator(DesiredPitch, DesiredYaw, 0.f);
	ApplyRelRot();
	ClampYaw();
}
void UFreeLookSpringArmComponent::TickRotateSmooth(float dt){
	// Skip if no need to rotate
	if (SMath::NearlyEqual(DesiredPitch + DesiredYaw, RelRot.Pitch + RelRot.Yaw, SMALL_NUMBER))
		return;

	float RotateMax = dt * SmoothRotateRate;

	RelRot.Yaw = SMath::MoveTo(RelRot.Yaw, DesiredYaw, RotateMax);
	RelRot.Pitch = SMath::MoveTo(RelRot.Pitch, DesiredPitch, RotateMax);

	ApplyRelRot();
	ClampYaw();
}

void UFreeLookSpringArmComponent::TickZoomDirect(){
	TargetArmLength = DesiredLength;
}
void UFreeLookSpringArmComponent::TickZoomSmooth(float dt){
	TargetArmLength = SMath::MoveTo(TargetArmLength, DesiredLength, SmoothZoomRate*dt);
}

void UFreeLookSpringArmComponent::InitRotateAndZoom() {
	TargetArmLength = DesiredLength = InitLength;
	RelRot = FRotator(InitPitch, 0.f, 0.f);
	DesiredPitch = InitPitch;
	DesiredYaw = 0;
	ApplyRelRot();
}
