// By Sodium
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "../SCommon/SCommon.h"
#include "FreeLookSpringArmComponent.generated.h"

/**
 * Free-look spring arm is a spring arm that can receive rotating, pitching and zooming input.
 * Designed for character control in third-person game. 
 */

UCLASS()
class L2WORLD_API UFreeLookSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	/* Rotator Params */

private:

	// Initial rotation i.e. world rotation when rotation is identity.
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Initial Pitch"), category = "FreeLook")
	float InitPitch = 30.f;

	// Initial rotation: defines the "forward orientation" of the camera when RelRot==Identity.
	// Usually only contains yaw. Do not reset it unless necessary.
	FObjectRotator InitRot = FObjectRotator();

	// Relative rotation to InitRot
	// DO NOT override this value with real rotation! Always set this value first and apply it to real rotation.
	// Also, this value never allows non-zero roll or abs(pitch)>=90.
	FRotator RelRot = FRotator();

	// Apply RelRot value to real rotation
	void ApplyRelRot();

	// Clamp yaw in (-180, 180]. This is intended to avoid yaw overflow.
	void ClampYaw();


public:

	FRotator GetRelRot(){ return RelRot; };

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Camera Rotation (Free Look Spring Arm)"), Category = SpringArm)
	static FRotator StaticGetRelRot(UFreeLookSpringArmComponent* Target) { return Target->GetRelRot(); };

	// Set camera rotation. If force set, desired rotation will be cleared.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Camera Rotation (Free Look Spring Arm)"), Category = SpringArm)
	void SetRelRot(float NewYaw, float NewPitch, bool ForceSet = true);

	// Add camera rotation offset. If force set, desired rotation will be cleared.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Camera Rotation (Free Look Spring Arm)"), Category = SpringArm)
	void AddRelRot(float DeltaYaw, float DeltaPitch, bool ForceSet = true);

	// Reset initial rotatio. Not recommended unless very necessary.
	// If keep vertical, pitch and yaw will keep zero.
	// Warning: if it is called in runtime, a vision jump will occur.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Camera Initial Rotation (Free Look Spring Arm)"), Category = SpringArm)
	void ResetInitRot(FRotator NewInitialRotation, bool bKeepVertical = true);


public:
	/* Rotate */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Mouse Rotation Sensitivity"), category = "FreeLook|Rotation")
	float YawSens_M = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Mouse Pitching Sensitivity"), category = "FreeLook|Rotation")
	float PitchSens_M = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Keyboard Rotation Sensitivity"), category = "FreeLook|Rotation")
	float YawSens_K = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Keyboard Pitching Sensitivity"), category = "FreeLook|Rotation")
	float PitchSens_K = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Smooth Rotation"), category = "FreeLook|Rotation")
	bool bEnableSmoothRotate = false;

	// Max rotation speed when smooth rotation enabled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Smooth Rotation Rate"), category = "FreeLook|Rotation")
	float SmoothRotateRate = 1.f;


	/* Zoom */
private:
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Initial Arm Length"), category = "FreeLook|Zoom")
	float InitLength = 300.f;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Arm Length"), category = "FreeLook|Zoom")
	float MaxLength = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Arm Length"), category = "FreeLook|Zoom")
	float MinLength = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Zoom Sensitivity"), category = "FreeLook|Zoom")
	float ZoomSens = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reverse Zoom"), category = "FreeLook|Zoom")
	bool bReverseZoom = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Smooth Zoom"), category = "FreeLook|Zoom")
	bool bEnableSmoothZoom = false;

	// Max rotation speed when smooth rotation enabled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Smooth Zoom Rate"), category = "FreeLook|Zoom")
	float SmoothZoomRate = 100.f;


	
	// Binding functions for input component
	void InputRotation_M(float val);
	void InputPitch_M(float val);
	void InputRotation_K(float val);
	void InputPitch_K(float val);
	void InputZoomIn();
	void InputZoomOut();




private:

	// For smoothing, record desired rotation
	float DesiredPitch = 30.f;
	float DesiredYaw = 0.f;
	float DesiredLength = 300.f;
	
	void TickRotateDirect();
	void TickRotateSmooth(float dt);
	void TickZoomDirect();
	void TickZoomSmooth(float dt);


	// Called only on begin play
	void InitRotateAndZoom();

	

};

typedef UFreeLookSpringArmComponent UFLSpringArm;