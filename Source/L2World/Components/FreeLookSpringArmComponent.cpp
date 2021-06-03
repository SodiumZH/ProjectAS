// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FreeLookSpringArmComponent.h"

void UFreeLookSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UFreeLookSpringArmComponent::InputRotation_M(float val) {
	DesiredYaw += (val*YawSens_M);
}
void UFreeLookSpringArmComponent::InputPitch_M(float val) {
	DesiredPitch += (val*PitchSens_M);
}
void UFreeLookSpringArmComponent::InputRotation_T(float val) {
	DesiredYaw += (val*YawSens_T);
}
void UFreeLookSpringArmComponent::InputPitch_T(float val) {
	DesiredPitch += (val*PitchSens_T);
}
void UFreeLookSpringArmComponent::InputZoomIn() {
	if(bReverseZoom)
		DesiredLength += ZoomSens;
	else DesiredLength -= ZoomSens;
}
void UFreeLookSpringArmComponent::InputZoomOut() {
	if (bReverseZoom)
		DesiredLength -= ZoomSens;
	else DesiredLength += ZoomSens;
}

float UFreeLookSpringArmComponent::TickRotateDirect(float dt) {

}
float UFreeLookSpringArmComponent::TickRotateSmooth(float dt){}
float UFreeLookSpringArmComponent::TickZoomDirect(float dt){}
float UFreeLookSpringArmComponent::TickZoomSmooth(float dt){}
