// By Sodium
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "../NaUtility/NaUtility.h"
#include "../NaGlobalHeader.h"
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
	float InitPitch = -30.f;

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
/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Keyboard Rotation Sensitivity"), category = "FreeLook|Rotation")
	float YawSens_K = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Keyboard Pitching Sensitivity"), category = "FreeLook|Rotation")
	float PitchSens_K = 1.f;
*/
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
	//void InputRotateLeft_K() { KRotLeft = true; };
	//void InputRotateLeftEnd_K() { KRotLeft = false; };
	//void InputRotateRight_K() { KRotRight = true; }
	//void InputRotateRightEnd_K() { KRotRight = false; };
	//void InputPitchUp_K() { KRotUp = true; };
	//void InputPitchUpEnd_K() { KRotUp = false; };
	//void InputPitchDown_K() { KRotDown = true; };
	//void InputPitchDownEnd_K() { KRotDown = false; };
	void InputZoomIn();
	void InputZoomOut();
	



private:

	// For smoothing, record desired rotation
	float DesiredPitch = 30.f;
	float DesiredYaw = 0.f;
	float DesiredLength = 300.f;
	
	//bool KRotLeft = false;
	//bool KRotRight = false;
	//bool KRotUp = false;
	//bool KRotDown = false;

	void TickRotateDirect();
	void TickRotateSmooth(float dt);
	void TickZoomDirect();
	void TickZoomSmooth(float dt);
//	void TickKeyboardRotate();
	void TickRotateAndZoom(float dt);

	// Called only on begin play
	void InitRotateAndZoom();

	/* Direction */

public:

	
	FVector GetForward() { return (FVector::ForwardVector * (InitRot + FObjectRotator(FRotator(0.f, RelRot.Yaw, 0.f)))); };
	FVector GetRight() { return (FVector::RightVector * (InitRot + FObjectRotator(FRotator(0.f, RelRot.Yaw, 0.f)))); };
	FVector GetUp() { return (FVector::UpVector * (InitRot + FObjectRotator(FRotator(0.f, RelRot.Yaw, 0.f)))); };

	// Get the vector toward the CHARACTER forward according to camera
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Camera Forward Vector (Free Look)"), Category = SpringArm)
	static FVector StaticGetForward(UFreeLookSpringArmComponent* Target) { return Target->GetForward(); };

	// Get the vector toward the CHARACTER right according to camera
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Camera Right Vector (Free Look)"), Category = SpringArm)
	static FVector StaticGetRight(UFreeLookSpringArmComponent* Target) { return Target->GetRight(); };

	// Get the vector toward the CHARACTER up according to camera
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Camera Up Vector (Free Look)"), Category = SpringArm)
	static FVector StaticGetUp(UFreeLookSpringArmComponent* Target) { return Target->GetUp(); };





};

typedef UFreeLookSpringArmComponent UFLSpringArm;

// Call this on header if FLSpringArm is used to enable input
#define DECLARE_FREE_LOOK_INPUT(SpringArmParam) \
	void InputZoomIn_SpringArmParam() {SpringArmParam->InputZoomIn();};\
	void InputZoomOut_SpringArmParam() {SpringArmParam->InputZoomOut();};\
	void InputRotation_M_SpringArmParam(float v) {SpringArmParam->InputRotation_M(v);};\
	void InputPitch_M_SpringArmParam(float v) {SpringArmParam->InputPitch_M(v);};


// Call this to setup input
#define SetupFreeLookInput(SelfClass, SpringArmParam, PlayerInputComponent, NameZoomIn, NameZoomOut, NameMouseRotate, NameMousePitch) \
	check(PlayerInputComponent);\
	PlayerInputComponent->BindAction(NameZoomIn, IE_Pressed, this, &SelfClass::InputZoomIn_SpringArmParam);\
	PlayerInputComponent->BindAction(NameZoomOut, IE_Pressed, this, &SelfClass::InputZoomOut_SpringArmParam);\
	PlayerInputComponent->BindAxis(NameMouseRotate, this, &SelfClass::InputRotation_M_SpringArmParam);\
	PlayerInputComponent->BindAxis(NameMousePitch, this, &SelfClass::InputPitch_M_SpringArmParam);

#define SetupFreeLookInput_Default(SelfClass, SpringArmParam, PlayerInputComponent) SetupFreeLookInput(SelfClass, SpringArmParam, PlayerInputComponent, TEXT("CameraZoomIn"), TEXT("CameraZoomOut"), TEXT("CameraMouseRotate"), TEXT("CameraMousePitch"))

