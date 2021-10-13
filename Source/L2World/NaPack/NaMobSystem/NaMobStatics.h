
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaMob.h"
#include "NaMobStatics.generated.h"

class ANaMobSkill;
class ANaMobSkillCollision;
class ANaMobWeapon;
class UTimeControlComponent;
class UNaMobPlayerComponent;

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

	/*** Component ***/

	//UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "", Keywords = ""), Category = "NaPack|MobSystem|Mob")
	// Return whether the mob is set as a player mob. It will not check if it is really controlled by a player.
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Is Player Mob", Keywords = "controlled"), Category = "NaPack|MobSystem|Mob|Components")
	static bool IsPlayerMob_BP(ANaMob* Target);
	
	// Get player component of a mob. If the mob is not a player mob, return null.
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast, DefaultToSelf, DisplayName = "Get Player Component (Mob)", Keywords = "player controller free look spring arm "), Category = "NaPack|MobSystem|Mob|Components")
	static void GetPlayerComponent_BP(ANaMob* Target, UNaMobPlayerComponent*& PlayerComponent);

	// Get time control component of a mob
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast, DefaultToSelf, DisplayName = "Get Time Control (Mob)", Keywords = "timeline time line time controller"), Category = "NaPack|MobSystem|Mob|Components")
	static void GetTimeControl_BP(ANaMob* Target, UTimeControlComponent*& TimeControl);

	// Get skill manager component of a mob
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast, DefaultToSelf, DisplayName = "Get Skill Manager (Mob)", Keywords = "skill controller"), Category = "NaPack|MobSystem|Mob|Components")
	static void GetSkillManager_BP(ANaMob* Target, UNaMobSkillManager*& SkillManager);


	/*** Animation Switch ***/
	
	/** Get the animation switch value of a name.
	Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	*/
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, BlueprintThreadSafe, DisplayName = "Get Animation State Switch Value", Keywords = "animation switch anim switch anim state animation state"), Category = "NaPack|MobSystem|AnimationSwitch")
	static void GetAnimStateSwitch_BP(ANaMob* Target, FName Key, bool& SwitchValue);

	/** Turn on an animation switch.
	* Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	* @Param Key Switch name.
	* @Param DeltaTime Time span that the switch keeps on. If this value is set non-positive, it will keep on until manually set off or overriden by next open action.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Open Animation State Switch", Keywords = "animation switch anim switch anim state animation state"), Category = "NaPack|MobSystem|AnimationSwitch")
	static void OpenAnimStateSwitch_BP(ANaMob* Target, FName Key, float DeltaTime = 0.1);

	/** Turn off an animation switch.
	* Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Close Animation State Switch", Keywords = "animation switch anim switch anim state animation state"), Category = "NaPack|MobSystem|AnimationSwitch")
	static void CloseAnimStateSwitch_BP(ANaMob* Target, FName Key);


	/*** Weapon Registeration ****/

	/* Get a weapon's register name on a mob. If the weapon is not registered, return empty name (""). */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Weapon Register Name", Keywords = "weapon name get name"), Category = "NaPack|MobSystem|Mob|Weapon")
	static void GetWeaponRegisterName_BP(ANaMob* Target, ANaMobWeapon* Weapon, FName& RegisterName);

	/* Get weapon from register name on a mob. Please note that empty name ("") is not a valid register name. */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Weapon from Register Name", Keywords = "weapon name get weapon from name"), Category = "NaPack|MobSystem|Mob|Weapon")
	static void GetWeaponFromRegisterName_BP(ANaMob* Target, FName RegisterName, ANaMobWeapon*& Weapon);





	/****************** Skill *********************/

	/* Get time control component of mob skill */
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast, DefaultToSelf, DisplayName = "Get Time Control (Mob Skill)"), Category = "NaPack|MobSystem|Skill")
	static void GetTimeControl_BP_Skill(ANaMobSkill* InSkill, UTimeControlComponent*& TimeControl);

	/* Get source mob of skill.
	* Warning: this result is invalid on begin play. If it's needed to call it on begin play, delay a short time (e.g. 0.01s) first.
	*/
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast, DefaultToSelf, DisplayName = "Get Source Mob (Mob Skill)"), Category = "NaPack|MobSystem|Skill")
	static void GetSource_BP_Skill(ANaMobSkill* InSkill, ANaMob*& Source);

	/* Get socket name of skill.
	* Warning: this result is invalid on begin play. If it's needed to call it on begin play, delay a short time (e.g. 0.01s) first.
	*/
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast, DefaultToSelf, DisplayName = "Get Socket Name (Mob Skill)"), Category = "NaPack|MobSystem|Skill")
	static void GetSocket_BP_Skill(ANaMobSkill* InSkill, FName& Socket);

	/* Get register name of skill.
	* Warning: this result is invalid on begin play. If it's needed to call it on begin play, delay a short time (e.g. 0.01s) first.
	*/
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast, DefaultToSelf, DisplayName = "Get Register Name (Mob Skill)"), Category = "NaPack|MobSystem|Skill")
	static void GetRegisterName_BP_Skill(ANaMobSkill* InSkill, FName& RegName);

	/** Generate a skill object from a mob.
	* @Param OutSkill Skill actor reference generated.
	* @Param SourceMob The mob as source of this skill.
	* @Param SkillClass Applied skill class.
	* @Param InTransform Relative transform.
	* @Param RegisterName Register name in source mob.
	* @Param Force If true, skill will be force spawned even if the register name is occupied in source mob. In this case the old registered skill will be overwritten.
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
		FName RegisterName,
		bool Force = false,
		USceneComponent* AttachToComponent = nullptr,
		FName SocketName = NAME_None,
		bool DoAttachment = true
	);

	/* Get skill from mob by register name. */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Skill By Register Name", Keywords = "register name reg name"), Category = "NaPack|MobSystem|Skill")
	static void GetSkillByRegisterName_BP(ANaMob* SourceMob, FName InRegisterName, ANaMobSkill*& Skill);

	/* Get skill's source mob and register name. */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Skill Registeration", Keywords = "register name reg name"), Category = "NaPack|MobSystem|Skill")
	static void GetSkillRegisteration_BP(ANaMobSkill* InSkill, ANaMob*& SourceMob, FName& RegisterName);

	/* Get collision set of mob skill. Collision set is the set of existing collisions of the skill.
	* This function will clear all invalid collisions before return, so it's safe for iteration.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Collision Set (Mob Skill)"), Category = "NaPack|MobSystem|Skill")
	static void GetCollisionSet_BP(ANaMobSkill* Target, TSet<ANaMobSkillCollision*>& CollisionSet);


	/* Skill Collision */

	/* Get source skill of skill collision. */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Source Skill (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void GetSourceSkill_BP(ANaMobSkillCollision* Target, ANaMobSkill*& SourceSkill);

	/* Get socket name of skill collision */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Socket Name (Skill Collision)"), Category = "NaPack|MobSystem|SkillCollision")
	static void GetSocketName_BP(ANaMobSkillCollision* Target, FName& SocketName);

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
		bool DoAttachment = true
	);


	/** Weapon **/

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Owner Mob (Weapon)"), Category = "NaPack|MobSystem|Weapon")
	static void GetOwnerMob_BP_Weapon(ANaMobWeapon* InWeapon, ANaMob*& OwnerMob);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Has Owner (Weapon)", Keywords = "is owned"), Category = "NaPack|MobSystem|Weapon")
	static void HasOwner_BP_Weapon(ANaMobWeapon* InWeapon, bool& HasOwner);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Register Name (Weapon)", Keywords = "reg name regname"), Category = "NaPack|MobSystem|Weapon")
	static void GetRegName_BP_Weapon(ANaMobWeapon* InWeapon, FName& RegName);

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, DisplayName = "Get Socket Name (Weapon)", Keywords = "reg name regname"), Category = "NaPack|MobSystem|Weapon")
	static void GetSocketName_BP_Weapon(ANaMobWeapon* InWeapon, FName& SocketName);
};



