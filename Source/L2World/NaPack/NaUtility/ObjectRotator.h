#pragma once
// Created by Sodium @ Sodium3D Team

/**
* FObjectRotator is designed to be a safer rotation expression than any conventional representations of 3D rotation, including quaternions, (Eular) rotators ,matrices and vector-roll pairs (usually used to define bullets).
* It is intended to avoid all defects/unsafety of other rotation expressions, including zero/unnormalized values of quaternions, gimbal lock of eular rotators and vector-roll pairs, orthogonal-normal limitation and unintutivity of rotation matrices.
* It contains a quaternion as actural representation. However, any return value of FObjectRotator will never allow this quaternion zero or not normal.
* The quaternion (Named InteriorQuat) is private. There is only several methods to directly access InteriorQuat, any of which will check the validity of the value.
* Only constructors are allowed to directly set InteriorQuat, and only function Quat() is allowed to directly get InteriorQuat, which return the quat value.
* In any other functions, if accession to InteriorQuat is needed, use Quat() to get and constructors to set.
* Of cource many other functions may need get or set InteriorQuat to use or generate FObjectRotator struct, but all of them should be indirect, i.e. use Quat() to transform it to quaternion(which is equal to InteriorQuat).
* When you're coding using this struct, please forget InteriorQuat. Consider this a black box which can convert from and to any form of rotation.
*
* Operators are overloaded to directly rotate Actors and SceneComponents, and of course vectors.
* In descriptions below, FObjectRotator is called ORot.
* Zero rotation: ORot0 = FObjectRotator(), means no rotation (i.e. FQuat::Identity, real 1 as quat, unit matrix or FRotator(0,0,0)).
* ORot1 + ORot2: means a overall rotation that firstly ORot1, then ORot2. Not exchangable! (+= is also usable which will set value of ORot1) 
* -ORot: means inverse of ORot. Obviously -ORot + ORot, ORot + (-ORot) both equal to zero.
* ORot1 - ORot2: equal to ORot1 + (-ORot2). (+= is also usable which will set value of ORot1) 
* ORot1 / ORot2: means the relative rotation of ORot1 to ORot2. Equals to -ORot2 + ORot1.
* Vec * ORot: Return result Vec rotated with ORot. This will not set Vec value directly.
* Target *= ORot: Add a rotation of ORot to target. Target can be a vector, a vector array or an object.
* Target << ORot: Set target's rotation to ORot. If target is a vector, zero rotation will be (1,0,0). If target is an object, set its world rotation, no sweep.
* ORot << Target: Set ORot to target's rotation. If target is a vector, it will get it's orientation rotation with roll=0. If target is an object, get its world rotation.
* Comp <= ORot: Set comp(Scene Component)'s relative rotation to ORot. Relative to it's attached parent, and if parent unexists, to world.
* ORot <= Comp: Set ORot to comp's relative rotation.
* Tip: consider << and <= as arrows.
*
* FObjectRotator（对象旋转）被设计为一个比其他所有内置3D旋转表达更安全的旋转表达，包括四元数、旋转体（欧拉角）、旋转矩阵以及向量-自旋对（常用于定义子弹）。
* 其被期望避免所有其他3D旋转表达的缺陷和不安全性，包括四元数的零和非归一化值、欧拉旋转体和向量-自旋对的万向锁现象，以及旋转矩阵的正交归一化限制及不直观性。
* 其中包含一个四元数作为实际的旋转表达。然而，FObjectRotator永远不会使这个四元数为零或未归一化，即不能表达一个旋转。
* 这个四元数（称为内部四元数InteriorQuat）为私有。仅有少数的几个方法能够直接访问内部四元数，其中每一个都会严格检查该值的有效性。
* 仅构造函数允许直接修改内部四元数。仅函数Quat()允许直接获取内部四元数，并返回这个值本身。
* 在其他函数中，如果需要访问内部四元数，请使用Quat()获取，使用构造函数修改。
* 当然很多其他函数需要访问内部四元数，以使用或生成FObjectRotator结构体，但是都应该是间接的，即使用Quat()。
* 因此当你在编程中使用这个结构体，请忘掉内部四元数。将这个结构体视为一个可以与所有旋转表达互相转换的黑箱。
*
* 操作符被重载得可以直接操作Actor和SceneComponent，当然还有向量。下述中FObjectRotator简作ORot。
* 零旋转：ORot_Identity == FObjectRotator() 代表不作旋转。即FQuat::Identity、实数1、单位矩阵或FRotator(0,0,0)。
* ORot = Target：将ORot的值设为目标的旋转。如果目标是向量，将取其方向旋转并设roll为零。如果目标是旋转表达，取其等值；如果目标是物体，取其世界旋转。
* ORot1 + ORot2: 相当于先做ORot1后做ORot2的总旋转。不满足交换律！！！ "+="也可以使用，设置ORot1的值。
* -ORot：表示ORot的逆旋转，即ORot.Inverse()。
* ORot1 - ORot2: 等同于ORot1 + (-ORot2)。"-="也可以使用，设置ORot1的值。
* ORot1 / ORot2: 表示ORot1相对ORot2的相对旋转。相当于-ORot2+ORot1.
* Vec * ORot： 返回向量Vec被ORot旋转的结果。此操作不会设置Vec的值。
* Target *= ORot: 对目标进行ORot旋转。目标可以是向量、向量数组也可以是物体（AActor*或USceneComponent*）。当是物体时将添加旋转，即在原旋转基础上再增加ORot旋转。
* Target << ORot：将目标的旋转设置为ORot。如果目标是向量，零旋转将被视为(1,0,0)。如果目标是物体，设置世界旋转（不sweep）。

* Comp <= ORot:仅用于组件。将组件Comp相对其直接父组件的相对旋转设置为ORot。如果其父组件不存在，设置世界旋转。
* ORot <= Comp：将ORot设为Comp的相对旋转（直接父组件？直接父组件旋转：世界旋转）。
* 提示：将<<与<=视作箭头。
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "../NaGlobalHeader.h"
#include "ObjectRotator.generated.h"

// Math error reporting macro
#define MathError(ID, FuncName, Info, Treat) UE_LOG(LogTemp, Error, TEXT("Math error(0x%04X):[%s()] %s. Treatment: %s."), ID, FuncName, Info, Treat)









USTRUCT(BlueprintType)
struct FObjectRotator {

	GENERATED_USTRUCT_BODY()

protected:

	// Actual quaternion for the rotation
	FQuat InteriorQuat;

public:

	/* Constructors */

	/* Only constructors are allowed to directly set InteriorQuat. Validity of InteriorQuat will be seriouly checked. (Always nonzero and normal) */

	// Default constructor to return a zero rotation
	FObjectRotator();
	static FObjectRotator Identity() { return FObjectRotator(); }; // Function to get identity

	// From FQuat. Input is no need to normalize, as it will be automatically normalized.
	FObjectRotator(FQuat InQuat);

	// From FRotator
	FObjectRotator(FRotator InRotator);

	// From orientation vector and roll. If vector is parallel to Z i.e. gimbal lock, define rotator yaw = 0
	// i.e. when X=(0,0,1), then Y=(0,1,0) & Z=(0,0,-1); when X=(0,0,-1) then Y=(0,1,0) and Z=(0,0,1)
	FObjectRotator(FVector InOrientation, float InRoll);

	// From vector only. This will set roll zero, just like FVector::ToOri.
	FObjectRotator(FVector InVec);

	// From actor world rotation
	FObjectRotator(AActor* InActor);

	// From component world rotation
	FObjectRotator(USceneComponent* InComp);

	/* Safety check and error report */

	// Check validity of an ObjectRotator. Those as return values of constructors or expressions are always legal and no need to call this function.
	// Needed if an ObjectRotator is generated from undesigned or unsafe actions, e.g. force type casting.
	bool IsLegal();
	static bool IsLegal(FObjectRotator Rot);

	// Make an ObjectRotator valid. 
	// If it's zero quaternion, transform it to identity; if it's unnormalized, normalize it.
	// Return legalized rotation (unit quaternion).
	FObjectRotator Legalize();
	static FObjectRotator GetLegalized(FObjectRotator& Rot);




	/* Additional functions to generate an ObjectRotator. They can only set InteriorQuat indirectly by calling constructors. */

	// FromAxisAngle, angle in degree, right-hand rule
	static FObjectRotator FromAxisAngle_d(FVector Axis, float AngleDegree);

	// FromAxisAngle, angle in radian, right-hand rule
	static FObjectRotator FromAxisAngle_r(FVector Axis, float AngleRadian);

	// Return rotation which applies to V1 resulting V2.
	static FObjectRotator FromTwoVectors(FVector V1, FVector V2);

	// Transformation to other rotation representations

	// To Quaternion
	// This function is the only one which is allowed to read InteriorQuat. Any other functions must call Quat() instead of InteriorQuat.
	FQuat Quat();

	// To Rotator
	FRotator Rotator();

	// To Vector and roll
	void VecRoll(FVector& Vec, float& Roll);

	// To vector, discarding roll
	FVector Vector();

	// To coordinate
	void Coordinate(FVector& X, FVector& Y, FVector& Z);

	// To forward, up, right vectors
	FVector ForwardVector();
	FVector RightVector();
	FVector UpVector();


	/* Operations */

	// Get inverse operation of this rotation
	FObjectRotator Inverse();

	// Get overall result of rotations with this and NextRotation successively
	FObjectRotator Compose(FObjectRotator NextRotation);

	// Add Euler rotation. Pitch and Yaw are around world coordinate while Roll is around local. Angles in degree.
	// Return rotation result. This operation do not set input variable.
	FObjectRotator AddEuler(float Pitch = 0.f, float Yaw = 0.f, float Roll = 0.f);

	// Remove roll(rotation toward own forward axis) of an objrotator.
	FObjectRotator DeRoll();
	static FObjectRotator DeRoll(FObjectRotator In);

	// Transform world rotation to relative. This do not consider scale of objects.
	static FObjectRotator WorldToRelative(FObjectRotator InWorldRot, FObjectRotator RelTo);
	static FObjectRotator WorldToRelative(FObjectRotator InWorldRot, AActor* RelTo);
	static FObjectRotator WorldToRelative(FObjectRotator InWorldRot, USceneComponent* RelTo);

	// Transform relative rotation to world. This do not consider scale of objects.
	static FObjectRotator RelativeToWorld(FObjectRotator InRelRot, FObjectRotator RelTo);
	static FObjectRotator RelativeToWorld(FObjectRotator InRelRot, AActor* RelTo);
	static FObjectRotator RelativeToWorld(FObjectRotator InRelRot, USceneComponent* RelTo);





	/* Vector operations */

	// Apply this rotation to a vector
	FVector Rotate(FVector Vec);

	// Apply this rotation to a vector array
	void Rotate(TArray<FVector>& OutVecArray, const TArray<FVector>& InVecArray);

	// Transform world vector to relative. This do not consider scale of objects.
	static FVector WorldToRelative(FVector InWorldVec, FObjectRotator RelTo);
	static FVector WorldToRelative(FVector InWorldVec, AActor* RelTo);
	static FVector WorldToRelative(FVector InWorldVec, USceneComponent* RelTo);

	// Transform relative vector to world. This do not consider scale of objects.
	static FVector RelativeToWorld(FVector InRelVec, FObjectRotator RelTo);
	static FVector RelativeToWorld(FVector InRelVec, AActor* RelTo);
	static FVector RelativeToWorld(FVector InRelVec, USceneComponent* RelTo);

	












	/* Operations toward objects */

	/* Get Rotation as ORot */

	static FObjectRotator GetWorldRotation_ORot(AActor* Target);
	static FObjectRotator GetWorldRotation_ORot(USceneComponent* Target);

	static FObjectRotator GetRelativeRotation_ORot(AActor* Target, AActor* RelativeTo);		// Relative to an actor; if RelativeTo is nullptr return world rotation
	static FObjectRotator GetRelativeRotation_ORot(AActor* Target, USceneComponent* RelativeTo);	// Relative to a component; if RelativeTo is nullptr return world rotation
	static FObjectRotator GetRelativeRotation_ORot(USceneComponent* Target);	// Relative to parent component
	static FObjectRotator GetRelativeRotation_ORot(USceneComponent* Target, AActor* RelativeTo);	// Relative to an actor; if RelativeTo is nullptr return world rotation
	static FObjectRotator GetRelativeRotation_ORot(USceneComponent* Target, USceneComponent* RelativeTo);	// Relative to a component; if RelativeTo is nullptr return world rotation


	/* Add or set Rotation from ORot */

	// Add this rotation to an actor
	void AddWorldRotation_ORot(AActor* Target, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);

	// Add this rotation to a scene component
	void AddWorldRotation_ORot(USceneComponent* Target, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);

	// Set this rotation to an actor, relative to another actor
	void SetWorldRotation_ORot(AActor* Target, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);
	void SetWorldRotation_ORot(USceneComponent* Target, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);
	void AddRelativeRotation_ORot(AActor* Target, AActor* RelativeTo, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to an actor
	void AddRelativeRotation_ORot(AActor* Target, USceneComponent* RelativeTo, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to a component
	void AddRelativeRotation_ORot(USceneComponent* Target, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to parent component
	void AddRelativeRotation_ORot(USceneComponent* Target, AActor* RelativeTo, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to an actor
	void AddRelativeRotation_ORot(USceneComponent* Target, USceneComponent* RelativeTo, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to a component

	void SetRelativeRotation_ORot(AActor* Target, AActor* RelativeTo, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to an actor
	void SetRelativeRotation_ORot(AActor* Target, USceneComponent* RelativeTo, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to a component
	void SetRelativeRotation_ORot(USceneComponent* Target, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to parent component
	void SetRelativeRotation_ORot(USceneComponent* Target, AActor* RelativeTo, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);	// Relative to an actor
	void SetRelativeRotation_ORot(USceneComponent* Target, USceneComponent* RelativeTo, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);// Relative to a component






	/* Operators overload */

	/** Pure mathematical */
	//* Pure ObjectRotator
	// Operator =
	FObjectRotator operator=(FRotator Rot);
	FObjectRotator operator=(FQuat Quat);
	FObjectRotator operator=(FVector Vec);
	FObjectRotator operator=(AActor* Actor);
	FObjectRotator operator=(USceneComponent* Comp);

	// Return result of overall rotation with this and Next successively
	// Not exchangable!!
	FObjectRotator operator+(const FObjectRotator& Next);

	// this = this + next
	FObjectRotator operator+=(const FObjectRotator& Next);

	// return this + next.Inverse()
	FObjectRotator operator-(const FObjectRotator Next);

	// this = this - next
	FObjectRotator operator-=(const FObjectRotator Next);

	// equal to Inverse()
	FObjectRotator operator-();

	// "A/B" means rotation of A relative to B, in which A and B are world rotations or relative rotations to a same rotation. 
	FObjectRotator operator/(FObjectRotator RelTo);

	// this = this / RelTo
	FObjectRotator operator/= (FObjectRotator RelTo);

	//* Vector operations

	// Rotate a vector with a rotation, return result
	friend FVector operator*(FVector Vec, FObjectRotator Rot);

	// Vec = Vec * Rot
	friend FVector operator*=(FVector& Vec, FObjectRotator Rot);

	// Rotate a vector array
	friend void operator*=(TArray<FVector>& VecArray, FObjectRotator Rot);

	// Set Vec the orientation vector of Rot. Warning: this will lose roll dimension.
	friend FVector operator<<(FVector& Vec, FObjectRotator Rot);

	// Reverse version of operator<<
	void operator>>(FVector& Vec);


	/** Object operations */

	// Add world rotation, no sweep
	// Return self, so you can use Actor*=Rot1*=Rot2*=...
	friend AActor* operator*=(AActor* Target, FObjectRotator Rot);

	// Add world rotation, no sweep
	// Return self
	friend USceneComponent* operator*=(USceneComponent* Target, FObjectRotator Rot);

	// Set world rotation, no sweep
	friend AActor* operator<<(AActor* Target, FObjectRotator Rot);

	// Set component world rotation, no sweep
	friend USceneComponent* operator<<(USceneComponent*, FObjectRotator Rot);

	// Set component relative rotation to parent, no sweep
	friend USceneComponent* operator<=(USceneComponent* Target, FObjectRotator Rot);

	// Reverse version of operator<<
	void operator>>(AActor* Target);
	// Reverse version of operator<<
	void operator>>(USceneComponent* Target);


};

