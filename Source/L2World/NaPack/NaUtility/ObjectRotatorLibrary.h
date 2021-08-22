// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../NaGlobalHeader.h"
#include "ObjectRotator.h"
#include "ObjectRotatorLibrary.generated.h"

/**
 * This BP Library contains all BP functions relative to ObjectRotator.
 */
UCLASS()
class NAPACK_API UObjectRotatorLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
	
public:

	/** Conv to ObjectRotator */

	// Get Identity object rotator
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Identity (Object Rotator)", CompactNodeTitle = "0", Keywords = "0 identity"), Category = "Utilities|ObjectRotator")
	static FObjectRotator IdentityObjRot() { return FObjectRotator(); };

	// Convert rotator to object rotator
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToObjectRotator (Rotator)", CompactNodeTitle = "->", BlueprintAutocast, Keywords="to object rotation"), Category = "Utilities|ObjectRotator")
	static FObjectRotator Conv_RotatorToObjRot(FRotator InRot) { return FObjectRotator(InRot); };

	// Convert quat to object rotator
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToObjectRotator (Quat)", CompactNodeTitle = "->", BlueprintAutocast, Keywords = "to object rotation"), Category = "Utilities|ObjectRotator")
	static FObjectRotator Conv_QuatToObjRot(FQuat InQuat) { return FObjectRotator(InQuat); };

	// Convert vector to object rotator; set roll zero
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToObjectRotator (Vector)", CompactNodeTitle = "->", BlueprintAutocast, Keywords = "to object rotation vector"), Category = "Utilities|ObjectRotator")
	static FObjectRotator Conv_VecToObjRot(FVector InVec) { return FObjectRotator(InVec); };

	// Convert vector to object rotator, with additional roll
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToObjectRotator (Vector & Roll)", CompactNodeTitle = "->", Keywords = "to object rotation vector roll"), Category = "Utilities|ObjectRotator")
	static FObjectRotator Conv_OriRollToObjRot(FVector Orientation, float Roll) { return FObjectRotator(Orientation, Roll); };

	// Get object rotator from axis and angle in degree
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToObjectRotator (Axis & Angle in Degree)", Keywords = "to object rotation axis angle degree"), Category = "Utilities|ObjectRotator")
	static FObjectRotator ToObjRot_AxisAngleDeg(FVector Axis, float AngleInDegree) { return FObjectRotator::FromAxisAngle_d(Axis, AngleInDegree); };

	// Get object rotator from axis and angle in radian
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToObjectRotator (Axis & Angle in Radian)", Keywords = "to object rotation axis angle radian"), Category = "Utilities|ObjectRotator")
	static FObjectRotator ToObjRot_AxisAngleRad(FVector Axis, float AngleInRadian) { return FObjectRotator::FromAxisAngle_r(Axis, AngleInRadian); };

	/** 
	* Rotate source vector to result vector and get the object rotator of this rotation 
	* @Param Source Vector to be rotated.
	* @Param Result Vector after rotation.
	* @return Rotation of this operation as object rotator.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToObjectRotator (VectorRotating)", Keywords = "to object rotation rotate vectors two"), Category = "Utilities|ObjectRotator")
	static FObjectRotator ToObjRot_TwoVectors(FVector Source, FVector Result) { return FObjectRotator::FromTwoVectors(Source, Result); };

	// Get actor rotation as object rotator
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetActorRotation (ObjectRotator)", Keywords = "get actor object rotation"), Category = "Utilities|ObjectRotator")
	static FObjectRotator GetActorObjRot(AActor* InActor) { return FObjectRotator(InActor); };

	// Get component world rotation as object rotator
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetComponentWorldRotation (ObjectRotator)", Keywords = "get component world object rotation"), Category = "Utilities|ObjectRotator")
	static FObjectRotator GetComponentWorldObjRot(USceneComponent* InComponent) { return FObjectRotator(InComponent); };

	// Get component relative rotation to its direct parent as object rotator. If no direct parent, get world rotation.
	//UFUNCTION(BlueprintPure, meta = (DisplayName = "GetComponentRelativeRotation(ObjectRotator)", Keywords = "get component relative object rotation"), Category = "Utilities|ObjectRotator")
	//static FObjectRotator GetComponentRelObjRot(USceneComponent* InComponent) {
	//	FObjectRotator rot;
	//	rot <= InComponent;
	//	return rot; 
	//};


	/** From object rotator */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToRotator (ObjectRotator)", CompactNodeTitle = "->", BlueprintAutocast, Keywords = "to rotation"), Category = "Utilities|ObjectRotator")
	static FRotator Conv_ObjRotToRotator(FObjectRotator InObjRot) { return InObjRot.Rotator(); };

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToQuat (ObjectRotator)", CompactNodeTitle = "->", BlueprintAutocast, Keywords = "to quaterion"), Category = "Utilities|ObjectRotator")
	static FQuat Conv_ObjRotToQuat(FObjectRotator InObjRot) { return InObjRot.Quat(); };

	/** Pure math operations on obj rot */

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Compose (ObjectRotator)", CompactNodeTitle = "+", Keywords = "add compose +"), Category = "Utilities|ObjectRotator")
	static FObjectRotator ComposeObjRot(FObjectRotator First, FObjectRotator Next) { return First + Next; };

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Compose Inversed (ObjectRotator)", CompactNodeTitle = "-", Keywords = "substract minus -"), Category = "Utilities|ObjectRotator")
	static FObjectRotator ComposeInvObjRot(FObjectRotator First, FObjectRotator Next) { return First - Next; };

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Inverse (ObjectRotator)", CompactNodeTitle = "-", Keywords = "negative inverse -"), Category = "Utilities|ObjectRotator")
	static FObjectRotator InverseObjRot(FObjectRotator InRot) { return -InRot; };

	

};
