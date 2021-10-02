
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaMob.h"
#include "NaMobStatics.generated.h"

class ANaMobSkill;
class ANaMobSkillCollision;
class ANaMobWeapon;
class UTimeControlComponent;

UCLASS()
class NAPACK_API UNaMobStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/****************** World Constants *********************/
	
	// Get world basic walking speed. To set this value, you need to change the macro value in "NaMobData.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem|Mob|Constants")
	static void GetMobBasicWalkSpeed(float& Value) { Value = MOB_BASIC_WALK_SPEED; };

	// Get world basic acceleration. To set this value, you need to change the macro value in "NaMobData.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem|Mob|Constants")
	static void GetMobBasicAcceleration(float& Value) { Value = MOB_BASIC_ACCEL; };

	// Get world basic jump Z velocity. To set this value, you need to change the macro value in "NaMobData.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem|Mob|Constants")
	static void GetMobBasicJumpZ_Velocity(float& Value) { Value = MOB_BASIC_JUMP_Z_VELOCITY; };

	// Get mob default height. To set this value, you need to change the macro value in "NaMobData.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem|Mob|Constants")
	static void GetMobDefaultHeight(float& Value) { Value = MOB_DEFAULT_HEIGHT; };

	// Get mob default diameter. To set this value, you need to change the macro value in "NaMobData.h".
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem|Mob|Constants")
	static void GetMobDefaultDiameter(float& Value) { Value = MOB_DEFAULT_DIAMETER; };


	/****************	Mob	  *******************/

	/* Get a weapon's register name on a mob. If the weapon is not registered, return empty name (""). */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Weapon Register Name", Keywords = "weapon name get name"), Category = "NaPack|MobSystem|Mob|Weapon")
	static void GetWeaponRegisterName_BP(ANaMob* Target, ANaMobWeapon* Weapon, FName& RegisterName);

	/* Get weapon from register name on a mob. Please note that empty name ("") is not a valid register name. */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Weapon from Register Name", Keywords = "weapon name get weapon from name"), Category = "NaPack|MobSystem|Mob|Weapon")
	static void GetWeaponFromRegisterName_BP(ANaMob* Target, FName RegisterName, ANaMobWeapon*& Weapon);

	/****************** Skill *********************/

	/* Get time control component of mob skill */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Time Control (Mob Skill)"), Category = "NaPack|MobSystem|Skill")
	static void GetTimeControl_BP_Skill(ANaMobSkill* InSkill, UTimeControlComponent*& TimeControl);

	/** Generate a skill object from a mob.
	* @Param OutSkill Skill actor reference generated.
	* @Param SourceMob The mob as source of this skill.
	* @Param SkillClass Applied skill class.
	* @Param InTransform Relative transform.
	* @Param SocketName Socket of attachment of this skill.
	* @Param AttachToComponent Component of actor this skill should attach. If this param is left null, skill will attach to the root component.
	* @Param DoAttachment If set false, the skill actor will not attach to anything and generate with world transform.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Use Skill by Class"), Category = "NaPack|MobSystem|Skill")
	static void UseSkillByClass_BP(
		ANaMobSkill*& OutSkill,
		ANaMob* SourceMob,
		TSubclassOf<ANaMobSkill> SkillClass,
		const FTransform & InTransform,
		USceneComponent* AttachToComponent = nullptr,
		FName SocketName = NAME_None,
		bool DoAttachment = true
	);

	/* Get collision set of mob skill. Collision set is the set of existing collisions of the skill.
	* This function will clear all invalid collisions before return, so it's safe for iteration.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Collision Set (Mob Skill)"), Category = "NaPack|MobSystem|Skill")
	static void GetCollisionSet_BP(ANaMobSkill* Target, TSet<ANaMobSkillCollision*>& CollisionSet);

	/* Skill Collision */

	/* Get collision root of skill collision. 
	* Set transform of this component instead of the collision body.
	*/
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Collision Root(Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void GetCollisionRoot_BP(ANaMobSkillCollision* Target, USceneComponent*& CollisionRoot);

	/* Get collision component of skill collision. 
	* Collision component can be a sphere, capsule, box or static mesh component.
	* Return value is in primitive component. Cast if specific class is needed.
	*/
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Collision (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void GetCollision_BP(ANaMobSkillCollision* Target, UPrimitiveComponent*& Collision);

	/* Get source skill of skill collision. */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Source Skill (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void GetSourceSkill_BP(ANaMobSkillCollision* Target, ANaMobSkill*& SourceSkill);

	/* Get socket name of skill collision */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Socket Name (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void GetSocketName_BP(ANaMobSkillCollision* Target, FName& SocketName);

	/* Get collision half size.
	* Warning: this result is not valid for static mesh collision.
	* @Param X: Box half X size, capsule radius or sphere radius.
	* @Param Y: Box half Y size. Invalid for sphere and capsule.
	* @Param Z: InZ Box half Z size or capsule half height. Invalid for sphere.
	*/
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Half Size (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void GetHalfSize_BP(ANaMobSkillCollision* Target, float& X, float& Y, float& Z);

	/* Get collision half size in vector.
	* Warning: this result is not valid for static mesh collision.
	* HalfSize.X: Box half X size, capsule radius or sphere radius.
	* HalfSize.Y: Box half Y size. Invalid for sphere and capsule.
	* HalfSize.Z: InZ Box half Z size or capsule half height. Invalid for sphere.
	*/
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Half Size in Vector (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void GetHalfSize_BP_Vec(ANaMobSkillCollision* Target, FVector& HalfSize);

	/* Set collision half size.
	* This function doesn't work for static mesh collision. For static mesh please set actor transform directly.
	* @Param InX Box half X size, capsule radius or sphere radius.
	* @Param InY Box half Y size. Invalid for sphere and capsule.
	* @Param InZ Box half Z size or capsule half height. Invalid for sphere.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Set Half Size (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void SetHalfSize_BP(ANaMobSkillCollision* Target, float InX = 8.f, float InY = 8.f, float InZ = 16.f);

	/* Set collision half size with vector input.
	* This function doesn't work for static mesh collision. For static mesh please set actor transform directly.
	* InVec.X: Box half X size, capsule radius or sphere radius.
	* InVec.Y: Box half Y size. Invalid for sphere and capsule (just keep default).
	* InVec.Z: InZ Box half Z size or capsule half height. Invalid for sphere (just keep default).
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Set Half Size in Vector (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void SetHalfSize_BP_Vec(ANaMobSkillCollision* Target, FVector InVec = FVector(8.f, 8.f, 16.f));

	/** Make a collision from a skill.
	* @Param OutCollision Skill collision generated.
	* @Param SourceSkill The skill owning this collision.
	* @Param Class Applied skill collision class.
	* @Param InTransform Relative transform.
	* @Param AttachToComponent Component this collision should attach. If this param is left null, skill will attach to the root component.
	* @Param SocketName Socket of attachment of this skill.
	* @Param DoAttachment If set false, the skill actor will not attach to anything and generate with world transform.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Make Skill Collision by Class"), Category = "NaPack|MobSystem|SkillCollision")
	static void MakeCollisionByClass_BP(
		ANaMobSkillCollision*& OutCollision,
		ANaMobSkill* SourceSkill,
		TSubclassOf<ANaMobSkillCollision> Class,
		const FTransform & InTranform,
		USceneComponent* AttachToComponent = nullptr,
		FName Socket = NAME_None,
		float LifeSpan = 0.f,
		bool DoAttachment = true
	);


	/** Weapon **/
};



