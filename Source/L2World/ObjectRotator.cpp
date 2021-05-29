
#include "ObjectRotator.h"



// Constructors

FObjectRotator::FObjectRotator() {
	InteriorQuat = FQuat::Identity;
}

FObjectRotator::FObjectRotator(FQuat InQuat) {
	FQuat OutQuat = InQuat.GetNormalized();
	if (OutQuat.Size() <= 0.1f) {
		MathError(11, "FObjectRotator", "Initial quaternion is zero", "Return zero rotation");
		InteriorQuat = FQuat::Identity;
	}
	else InteriorQuat = OutQuat;
}

FObjectRotator::FObjectRotator(FRotator InRotator) {
	InteriorQuat = InRotator.Quaternion().GetNormalized();
}

FObjectRotator::FObjectRotator(FVector InOrientation, float InRoll) {
	FVector OriNormal = InOrientation.GetSafeNormal();

	// zero input => report error and return zero rotation
	if (OriNormal.Size() < 0.1f) {

		MathError(12, "FObjectRotator", "Initial orientation vector is zero", "Return zero rotation");

		InteriorQuat = FQuat::Identity; // Return zero rotation
	}
	else {
		FQuat QNoRoll = OriNormal.ToOrientationQuat().GetNormalized();
		FQuat QRoll = FQuat(OriNormal, InRoll*PI / 180.f).GetNormalized();
		if ((QRoll*QNoRoll).GetNormalized().Size() >= 0.1f)
			InteriorQuat = (QRoll*QNoRoll).GetNormalized();
		else InteriorQuat = FQuat::Identity;
	}
}

FObjectRotator::FObjectRotator(FVector InVec) {
	InteriorQuat = FObjectRotator(InVec, 0.f).Quat().GetNormalized();
}

FObjectRotator::FObjectRotator(AActor* InActor) {
	if (IsValid(InActor))
		InteriorQuat = InActor->GetActorQuat().GetNormalized();
	else InteriorQuat = FQuat::Identity;
}

FObjectRotator::FObjectRotator(USceneComponent* InComp) {
	if (IsValid(InComp))
		InteriorQuat = InComp->GetComponentQuat().GetNormalized();
	else InteriorQuat = FQuat::Identity;
}














// Validity

bool FObjectRotator::IsLegal() {
	return FMath::IsNearlyEqual(Quat().Size(), 1.f, SMALL_NUMBER);
}

bool FObjectRotator::IsLegal(FObjectRotator Rot) {
	return Rot.IsLegal();
}

FObjectRotator FObjectRotator::Legalize() {
	FObjectRotator Rot = FObjectRotator((*this).Quat().GetNormalized());
	if (Rot.Quat().Size() < 0.1)
		return FObjectRotator();
	else return Rot;
}

FObjectRotator FObjectRotator::GetLegalized(FObjectRotator & Rot) {
	Rot = Rot.Legalize();
	return Rot;
}
















// Other methods to generate FObjectRotator struct

FObjectRotator FObjectRotator::FromAxisAngle_d(FVector Axis, float AngleDegree) {

	return FromAxisAngle_r(Axis, FMath::DegreesToRadians(AngleDegree));

}

FObjectRotator FObjectRotator::FromAxisAngle_r(FVector Axis, float AngleRadian) {
	FVector AxisNormal = Axis.GetSafeNormal();
	if (AxisNormal.IsNearlyZero()) {
		MathError(13, "FromAxisAngle_d() or FromAxisAngle_r", "Axis is zero", "Return zero rotation");
		return FObjectRotator();
	}

	return FObjectRotator(FQuat(AxisNormal, AngleRadian));
}

FObjectRotator  FObjectRotator::FromTwoVectors(FVector V1, FVector V2) {
	FVector V1N = V1.GetSafeNormal();
	FVector V2N = V2.GetSafeNormal();
	FVector Axis;
	if (V1N.IsNearlyZero() || V2N.IsNearlyZero()) {
		MathError(14, "FromTwoVectors", "Received zero input", "Return zero rotation");
		return FObjectRotator();
	}
	if (FVector::Parallel(V1N, V2N)) {
		if (FVector::DotProduct(V1N, V2N) < 0) {
			MathError(15, "FromTwoVectors", "V1 and V2 is 180 degrees, rotation axis cannot be determined", "Assume axis is (1,0,0) or (0,1,0) if the former is parallel to inputs");
			Axis = FVector::Parallel(V1N, FVector(1, 0, 0)) ? FVector(0, 1, 0) : FVector(1, 0, 0);
			return FObjectRotator::FromAxisAngle_d(Axis, 180.f);
		} //180 degrees rotation
		else return FObjectRotator(); // no rotation
	}
	// normal cases
	else {
		Axis = (V1N ^ V2N).GetSafeNormal();
		float Angle_r = FMath::Acos(FVector::DotProduct(V1N, V2N));
		return FObjectRotator(FQuat(Axis, Angle_r));
	}

}





// Conversion to other formation of rot

FQuat FObjectRotator::Quat() {
	return InteriorQuat;
}

FRotator FObjectRotator::Rotator() {
	return Quat().Rotator();
}

// To Vector and roll
void FObjectRotator::VecRoll(FVector& Vec, float& Roll) {
	Vec = Rotate(FVector::ForwardVector);
	FQuat QNoRoll = Vec.ToOrientationQuat().GetNormalized();
	// QRoll*QNoRoll = k*Quat(), right-multiply by QNoRoll.Inverse() then normalize to get QRoll
	FQuat QRoll = Quat()*QNoRoll.Inverse().GetNormalized();
	//	float ErrorAngle = FMath::Acos(FVector(QRoll.X, QRoll.Y, QRoll.Z).GetSafeNormal() | Vec);
	Roll = (FVector(QRoll.X, QRoll.Y, QRoll.Z) | Vec) > 0 ? FMath::Acos(QRoll.W) : -FMath::Acos(QRoll.W);
}

FVector FObjectRotator::Vector() {
	return Quat().Vector();
}

void FObjectRotator::Coordinate(FVector& X, FVector& Y, FVector& Z) {
	X = Rotate(FVector::ForwardVector);
	Y = Rotate(FVector::RightVector);
	Z = Rotate(FVector::UpVector);
	return;
}

FVector FObjectRotator::ForwardVector(){
	return Rotate(FVector::ForwardVector);
}

FVector FObjectRotator::RightVector() {
	return Rotate(FVector::RightVector);
}

FVector FObjectRotator::UpVector() {
	return Rotate(FVector::UpVector);
}




/** Operations */

// 
FObjectRotator FObjectRotator::Inverse() {
	return FObjectRotator(Quat().Inverse());
}

FObjectRotator FObjectRotator::Compose(FObjectRotator NextRotation) {
	return FObjectRotator(NextRotation.Quat()*Quat());
}

FObjectRotator FObjectRotator::AddEuler(float Pitch, float Yaw, float Roll) {

	FObjectRotator Result = (*this);

	// Add pitch
	Result += FromAxisAngle_d(FVector::RightVector, Pitch);

	// Add yaw
	Result += FromAxisAngle_d(FVector::UpVector, Yaw);

	// Add roll
	Result += FromAxisAngle_d(Result.Vector(), Roll);

	return Result;

}

FObjectRotator FObjectRotator::WorldToRelative(FObjectRotator InWorldRot, FObjectRotator RelTo) {
	return -RelTo + InWorldRot;
}
FObjectRotator FObjectRotator::WorldToRelative(FObjectRotator InWorldRot, AActor* RelTo) {
	return -FObjectRotator(RelTo) + InWorldRot;
}
FObjectRotator FObjectRotator::WorldToRelative(FObjectRotator InWorldRot, USceneComponent* RelTo) {
	return  -FObjectRotator(RelTo) + InWorldRot;
}

FObjectRotator FObjectRotator::RelativeToWorld(FObjectRotator InRelRot, FObjectRotator RelTo) {
	return RelTo + InRelRot;
}
FObjectRotator FObjectRotator::RelativeToWorld(FObjectRotator InRelRot, AActor* RelTo) {
	return FObjectRotator(RelTo) + InRelRot;
}
FObjectRotator FObjectRotator::RelativeToWorld(FObjectRotator InRelRot, USceneComponent* RelTo) {
	return FObjectRotator(RelTo) + InRelRot;
}













// Rotate vectors
FVector FObjectRotator::Rotate(FVector Vec) {
	return Quat().RotateVector(Vec);
}

void FObjectRotator::Rotate(TArray<FVector>& OutVecArray, const TArray<FVector>& InVecArray) {
	OutVecArray.SetNum(InVecArray.Num());
	int i;
	for (i = 0; i < InVecArray.Num(); i++) {
		OutVecArray[i] = Rotate(InVecArray[i]);
	}
	return;
}

FVector FObjectRotator::WorldToRelative(FVector InWorldVec, FObjectRotator RelTo) {
	return (-RelTo).Rotate(InWorldVec);
}
FVector FObjectRotator::WorldToRelative(FVector InWorldVec, AActor* RelTo) {
	return (-FObjectRotator(RelTo)).Rotate(InWorldVec);
}
FVector FObjectRotator::WorldToRelative(FVector InWorldVec, USceneComponent* RelTo) {
	return (-FObjectRotator(RelTo)).Rotate(InWorldVec);
}

FVector FObjectRotator::RelativeToWorld(FVector InRelVec, FObjectRotator RelTo) {
	return RelTo.Rotate(InRelVec);
}
FVector FObjectRotator::RelativeToWorld(FVector InRelVec, AActor* RelTo) {
	return FObjectRotator(RelTo).Rotate(InRelVec);
}
FVector FObjectRotator::RelativeToWorld(FVector InRelVec, USceneComponent* RelTo) {
	return FObjectRotator(RelTo).Rotate(InRelVec);
}






/* Object related */


/* Get actor or component rotations */

FObjectRotator FObjectRotator::GetWorldRotation_ORot(AActor* Target) {
	// Validity check: invalid return zero rotation
	if (!IsValid(Target))
		return FObjectRotator();
	else return FObjectRotator(Target->GetActorQuat());
}

FObjectRotator FObjectRotator::GetWorldRotation_ORot(USceneComponent* Target) {
	// Validity check: invalid return zero rotation
	if (!IsValid(Target))
		return FObjectRotator();
	else return FObjectRotator(Target->GetComponentQuat());
}

FObjectRotator FObjectRotator::GetRelativeRotation_ORot(AActor* Target, AActor* RelativeTo) {
	// FObjectRotator Rot_T = FObjectRotator(Target->GetActorQuat());
	// FObjectRotator Rot_R = FObjectRotator(RelativeTo->GetActorQuat());
	// Rot_R + X = Rot_T
	// -Rot_R + Rot_R + X = -Rot_R + Rot_T
	// X = -Rot_R + Rot_T, which is defined Rot_T/Rot_R

	// Validity check: invalid target return zero rotation; invalid relative_to return world rotation
	if (!IsValid(Target))
		return FObjectRotator();
	else if (!IsValid(RelativeTo))
		return GetWorldRotation_ORot(Target);

	return FObjectRotator(Target->GetActorQuat()) - FObjectRotator(RelativeTo->GetActorQuat());
}

FObjectRotator FObjectRotator::GetRelativeRotation_ORot(AActor* Target, USceneComponent* RelativeTo) {
	// Validity check: invalid target return zero rotation; invalid relative_to return world rotation
	if (!IsValid(Target))
		return FObjectRotator();
	else if (!IsValid(RelativeTo))
		return GetWorldRotation_ORot(Target);

	return FObjectRotator(Target->GetActorQuat()) - FObjectRotator(RelativeTo->GetComponentQuat());

}

FObjectRotator FObjectRotator::GetRelativeRotation_ORot(USceneComponent* Target) {
	// Validity check: invalid target return zero rotation
	if (!IsValid(Target))
		return FObjectRotator();

	return  GetRelativeRotation_ORot(Target, Target->GetAttachParent());
}

FObjectRotator FObjectRotator::GetRelativeRotation_ORot(USceneComponent* Target, AActor* RelativeTo) {
	// Validity check: invalid target return zero rotation; invalid relative_to return world rotation
	if (!IsValid(Target))
		return FObjectRotator();
	else if (!IsValid(RelativeTo))
		return GetWorldRotation_ORot(Target);

	return FObjectRotator(Target->GetComponentQuat()) - FObjectRotator(RelativeTo->GetActorQuat());
}

FObjectRotator FObjectRotator::GetRelativeRotation_ORot(USceneComponent* Target, USceneComponent* RelativeTo) {
	// Validity check: invalid target return zero rotation; invalid relative_to return world rotation
	if (!IsValid(Target))
		return FObjectRotator();
	else if (!IsValid(RelativeTo))
		return GetWorldRotation_ORot(Target);

	return FObjectRotator(Target->GetComponentQuat()) - FObjectRotator(RelativeTo->GetComponentQuat());
}





/* Add or set actor or component rotations */

void FObjectRotator::AddWorldRotation_ORot(AActor* Target, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (IsValid(Target))
		Target->AddActorWorldRotation(Quat(), bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::AddWorldRotation_ORot(USceneComponent* Target, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (IsValid(Target))
		Target->AddWorldRotation(Quat(), bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::SetWorldRotation_ORot(AActor* Target, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (IsValid(Target) && IsValid(Target->GetRootComponent())) {
		Target->GetRootComponent()->SetWorldRotation(Quat(), bSweep, OutSweepHitResult, Teleport);
	}
	return;
}

void FObjectRotator::SetWorldRotation_ORot(USceneComponent* Target, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (IsValid(Target)) {
		Target->SetWorldRotation(Quat(), bSweep, OutSweepHitResult, Teleport);
	}
	return;
}

void FObjectRotator::AddRelativeRotation_ORot(AActor* Target, AActor* RelativeTo, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (!IsValid(Target))
		return;
	else if (!IsValid(RelativeTo)) {
		AddWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
		return;
	}

	FObjectRotator RelToRot = RelativeTo;
	(-RelToRot + (*this) + RelToRot).AddWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::AddRelativeRotation_ORot(AActor* Target, USceneComponent* RelativeTo, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (!IsValid(Target))
		return;
	else if (!IsValid(RelativeTo)) {
		AddWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
		return;
	}

	FObjectRotator RelToRot = RelativeTo;
	(-RelToRot + (*this) + RelToRot).AddWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::AddRelativeRotation_ORot(USceneComponent* Target, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	AddRelativeRotation_ORot(Target, Target->GetAttachParent(), bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::AddRelativeRotation_ORot(USceneComponent* Target, AActor* RelativeTo, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (!IsValid(Target))
		return;
	else if (!IsValid(RelativeTo)) {
		AddWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
		return;
	}

	FObjectRotator RelToRot = RelativeTo;
	(-RelToRot + (*this) + RelToRot).AddWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::AddRelativeRotation_ORot(USceneComponent* Target, USceneComponent* RelativeTo, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (!IsValid(Target))
		return;
	else if (!IsValid(RelativeTo)) {
		AddWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
		return;
	}

	FObjectRotator RelToRot = RelativeTo;
	(-RelToRot + (*this) + RelToRot).AddWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::SetRelativeRotation_ORot(AActor* Target, AActor* RelativeTo, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (!IsValid(Target))
		return;
	else if (!IsValid(RelativeTo)) {
		SetWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
		return;
	}

	(*this + GetWorldRotation_ORot(RelativeTo)).SetWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::SetRelativeRotation_ORot(AActor* Target, USceneComponent* RelativeTo, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (!IsValid(Target))
		return;
	else if (!IsValid(RelativeTo)) {
		SetWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
		return;
	}

	(*this + GetWorldRotation_ORot(RelativeTo)).SetWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
	return;

}

void FObjectRotator::SetRelativeRotation_ORot(USceneComponent* Target, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	SetRelativeRotation_ORot(Target, Target->GetAttachParent(), bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::SetRelativeRotation_ORot(USceneComponent* Target, AActor* RelativeTo, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (!IsValid(Target))
		return;
	else if (!IsValid(RelativeTo)) {
		SetWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
		return;
	}

	(*this + GetWorldRotation_ORot(RelativeTo)).SetWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
	return;
}

void FObjectRotator::SetRelativeRotation_ORot(USceneComponent* Target, USceneComponent* RelativeTo, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport) {
	if (!IsValid(Target))
		return;
	else if (!IsValid(RelativeTo)) {
		SetWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
		return;
	}

	(*this + GetWorldRotation_ORot(RelativeTo)).SetWorldRotation_ORot(Target, bSweep, OutSweepHitResult, Teleport);
	return;
}



/* Operators overload */

// Pure rotators
FObjectRotator FObjectRotator::operator=(FRotator Rot) {
	(*this) = FObjectRotator(Rot);
	return (*this);
}
FObjectRotator FObjectRotator::operator=(FQuat Quat) {
	(*this) = FObjectRotator(Quat);
	return (*this);
}
FObjectRotator FObjectRotator::operator=(FVector Vec) {
	(*this) = FObjectRotator(Vec);
	return (*this);
}
FObjectRotator FObjectRotator::operator=(AActor* Actor) {
	(*this) = FObjectRotator(Actor);
	return (*this);
}
FObjectRotator FObjectRotator::operator=(USceneComponent* Comp) {
	(*this) = FObjectRotator(Comp);
	return (*this);
}

FObjectRotator FObjectRotator::operator+(const FObjectRotator& Next) {
	return Compose(Next);
}

FObjectRotator FObjectRotator::operator+=(const FObjectRotator& Next) {

	*this = Compose(Next);
	return *this;

}

FObjectRotator FObjectRotator::operator-(FObjectRotator Next) {
	return Compose(Next.Inverse());
}

FObjectRotator FObjectRotator::operator-=(FObjectRotator Next) {
	*this = Compose(Next.Inverse());
	return *this;
}

FObjectRotator FObjectRotator::operator-() {
	return Inverse();
}

FObjectRotator FObjectRotator::operator/(FObjectRotator RelTo) {
	return WorldToRelative(*this, RelTo);
}

FObjectRotator FObjectRotator::operator/=(FObjectRotator RelTo) {
	*this = WorldToRelative(*this, RelTo);
	return *this;
}

/** Vector operations */
FVector operator*(FVector Vec, FObjectRotator Rot) {
	return Rot.Rotate(Vec);
}

FVector operator*=(FVector& Vec, FObjectRotator Rot) {
	Vec = Rot.Rotate(Vec);
	return Vec;
}

void operator*=(TArray<FVector>& VecArray, FObjectRotator Rot) {
	int i;
	for (i = 0; i < VecArray.Num(); i++) {
		VecArray[i] *= Rot;
	}
	return;
}

FVector operator<<(FVector& Vec, FObjectRotator Rot) {
	Vec = Rot.Rotate(FVector::ForwardVector);
	return Vec;
}

void FObjectRotator::operator>>(FVector& Vec) {
	Vec = Rotate(FVector::ForwardVector);
	return;
}

// Add & set objects

AActor* operator*=(AActor* Target, FObjectRotator Rot) {
	Rot.AddWorldRotation_ORot(Target);
	return Target;
}

USceneComponent* operator*=(USceneComponent* Target, FObjectRotator Rot) {
	Rot.AddWorldRotation_ORot(Target);
	return Target;
}

AActor* operator<<(AActor* Target, FObjectRotator Rot) {
	Rot.SetWorldRotation_ORot(Target);
	return Target;
}

USceneComponent* operator<<(USceneComponent* Target, FObjectRotator Rot) {
	Rot.SetWorldRotation_ORot(Target);
	return Target;
}

USceneComponent* operator<=(USceneComponent* Target, FObjectRotator Rot) {
	Rot.SetRelativeRotation_ORot(Target);
	return Target;
}

void FObjectRotator::operator>>(AActor* Target) {
	Target << (*this);
}

void FObjectRotator::operator>>(USceneComponent* Target) {
	Target << (*this);
}