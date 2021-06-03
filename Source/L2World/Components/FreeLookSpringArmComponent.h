// By Sodium
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "../LMathRef.h"
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



	/* Internal Params */

private:

	// Initial rotation i.e. world rotation (to parent) when rotation is identity.
	FObjectRotator InitRot= FObjectRotator();

	// Relative rotation to InitRot
	FRotator RelRot = FRotator();



	/* Camera Rotation Control */

public:

	/* Rotate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Mouse Rotation Sensitivity"), category = "FreeLook")
	float YawSens_M = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Mouse Pitching Sensitivity"), category = "FreeLook")
	float PitchSens_M = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Keyboard Rotation Sensitivity"), category = "FreeLook")
	float YawSens_K = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Keyboard Pitching Sensitivity"), category = "FreeLook")
	float PitchSens_K = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Smooth Rotation"), category = "FreeLook")
	bool bEnableSmoothRotate = false;

	// Max rotation speed when smooth rotation enabled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Smooth Rotation Rate"), category = "FreeLook")
	float SmoothRotateRate = 1.f;

	/* Zoom */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Zoom Sensitivity"), category = "FreeLook")
	float ZoomSens = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reverse Zoom"), category = "FreeLook")
	bool bReverseZoom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Smooth Zoom"), category = "FreeLook")
	bool bEnableSmoothZoom = false;

	// Max rotation speed when smooth rotation enabled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Smooth Zoom Rate"), category = "FreeLook")
	float SmoothZoomRate = 1.f;

	
	// Binding functions for input component
	void InputRotation_M(float val);
	void InputPitch_M(float val);
	void InputRotation_T(float val);
	void InputPitch_T(float val);
	void InputZoomIn();
	void InputZoomOut();

private:

	// For smoothing, record desired rotation
	float DesiredPitch;
	float DesiredYaw;
	float DesiredLength;
	
	float TickRotateDirect(float dt);
	float TickRotateSmooth(float dt);
	float TickZoomDirect(float dt);
	float TickZoomSmooth(float dt);

	
public:

};

typedef UFreeLookSpringArmComponent UFLSpringArm;