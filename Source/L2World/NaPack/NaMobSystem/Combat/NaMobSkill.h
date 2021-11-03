#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../NaGlobalHeader.h"
#include "NaMobSkillCollision.h"
#include "NaMobSkill.generated.h"

/**
*	NaMobSkill is a temporal actor generated when a mob uses a skill. 
*	MobSkill can bind to a given mesh socket to make the skill follow animation (e.g. sword attack => bind to "sword" socket)
*	This is also a manager of SkillMesh, which checks the hit result of the skill and transfer damage data to the target.
*
*	The overall actions of a skill releasing:
*	Mob(Source) release skill -> Generate a MobSkill actor(Skill) owned by Source -> Generate SkillMesh(s) owned by Skill
*	-> SkillMesh hit target(mob, static mesh, etc.) -> Send skill damage data to target -> send back skill-hit data back to Skill
*	-> send back skill result to Source
*/

class ANaMob;
class UTimeControlComponent;
class ANaMobSkillCollision;
struct FSkillCollisionHitReturn;

UENUM(BlueprintType)
enum class ESkillCollisionLocationType :uint8 {
	SCLT_RelToSkill		UMETA(DisplayName = "RelativeToSkill"),
	SCLT_RelToSource	UMETA(DisplayName = "RelativeToSource"),
	SCLT_RelToActor		UMETA(DisplayName = "RelativeToAttachedActor"),
	SCLT_RelToComponent	UMETA(DisplayName = "RelativeToAttachedComponent"),
	SCLT_World			UMETA(DisplayName = "World")
};

USTRUCT(BlueprintType)
struct FMobSkillUsageOptions {
	
	GENERATED_BODY()

public:
	
	// Keep default
	FMobSkillUsageOptions() {};

	// Initialize only from names. Switch times are 0.1s 
	FMobSkillUsageOptions(FName StartSwitch, FName EndSwitch) { StartAnimSwitch = StartSwitch; EndAnimSwitch = EndSwitch; };

	// Anim switch to open when using skill. "None" => don't open.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName StartAnimSwitch = NAME_None;

	// Time seconds to keep start anim switch on. Non-positive => always on. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StartSwitchTime = 0.1f;

	// Anim switch to open when skill finished (destroyed). "None" => don't open.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName EndAnimSwitch = NAME_None;

	// Time seconds to keep end anim switch on. Non-positive => always on. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float EndSwitchTime = 0.1f;

	// If true, the skill will not attach to anything and will use the input transform as world transform.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSkillNoAttachment = false;

	static FMobSkillUsageOptions Default;

};



UCLASS(BlueprintType)
class NAPACK_API ANaMobSkill : public AActor 
{

	GENERATED_BODY()

public:

	ANaMobSkill();

	virtual void OnConstruction(const FTransform & Trans) override;

	virtual void Tick(float DeltaTime) override;

	/* On skill initialized. 
	* Use this instead of begin play in mob skill, since begin play will cause initialization issues.
	* Will be called on the first frame, after a tick delta time from begin play.
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Initialized (Skill)"), Category = "NaPack|MobSystem|Skill")
	void Initialized();
	void Initialized_Implementation() {};

	


	/* Components */

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTimeControlComponent* TimeControl;

	virtual void BeginPlay() override;

public:

	UTimeControlComponent* GetTimeControl();

	/* Installation */

protected:

	//UPROPERTY(BlueprintReadOnly)
	ANaMob* Source = nullptr;

	//UPROPERTY(BlueprintReadOnly)
	FName Socket = NAME_None;

	//UPROPERTY(BlueprintReadOnly)
	FName RegisterName = NAME_None;

public:

	ANaMob* GetSource() { if (!Source) LogError("Source is invalid. For skills DO NOT use BeginPlay. Use OnInitialized instead.");	return Source; };

	FName GetSocket() { return Socket; };

	FName GetRegisterName() { if (RegisterName == NAME_None) LogError("Register name is invalid. For skills DO NOT use BeginPlay. Use OnInitialized instead."); return RegisterName; };

	/* Usage of skills */

	/** Generate a skill object from a mob.
	* @Param SourceMob The mob as source of this skill.
	* @Param SkillClass Applied skill class.
	* @Param InTransform Relative transform.
	* @Param SocketName Socket of attachment of this skill.
	* @Param AttachToComponent Component of actor this skill should attach. If this param is left null, skill will attach to the root component.
	* @Param DoAttachment If set false, the skill actor will not attach to anything and generate with world transform.
	*/
	static ANaMobSkill* UseSkillByClass(
		ANaMob* SourceMob,
		TSubclassOf<ANaMobSkill> SkillClass,
		const FTransform & InTransform,
		FName InRegisterName,
		const FMobSkillUsageOptions & Options,
		bool ForceSpawn = false,
		USceneComponent* AttachToComponent = nullptr,
		FName SocketName = NAME_None
	);

	// If true, this skill can be used when moving. You must correctly set the behavior when skill is used when moving to avoid errors. 
	// Generally set this value and LockMovementWhenUsing true can allow mob to use skill on movement and simutaneously stop moving.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bCanUseOnMoving = true;

	// If true, this skill can be used when jumping. You must correctly set the behavior when skill is used when jumping to avoid errors. 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bCanUseOnJumping = false;

	// If true, this skill can be used when other skill exists. Or any skill in usage will block it's spawning.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bCanOverlapOtherSkills = false;

	// If true, this skill can be used when skill which is subclass of this skill exists. Or any subclass skills in usage will block it's spawning.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bCanOverlapSubclassedSkills = false;

	// If true, this skill can be used for multiple times. Or an existing skill instance (right the same class of this skill, excluding subclasses) will block it's spawning.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bCanUseMultipleTimes = false;

	// If true, this skill blocks user's movement when used, until it's destroyed. And on destroyed it will unlock movement. (Warning when can overlap other skills!)
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bLockMovement = true;

	// If true and if this skill locks movement, when this skill is destroyed it will automatically unlock movement. Set false and handle movement manually when it can overlap other skills.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bUnlockMovementOnDestroyed = true;

	// If true, this skill blocks user's jump when used, until it's destroyed. And on destroyed it will unlock jump. (Warning when can overlap other skills!)
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bLockJump = true;

	// If true and if this skill locks jump, when this skill is destroyed it will automatically unlock jump. Set false and handle jump manually when it can overlap other skills.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MobSkill|Usage")
	bool bUnlockJumpWhenDestroyed = true;


	/* Collision check */

protected:

	TSet<ANaMobSkillCollision*> CollisionSet;

	// Remove all invalid collisions from set. Always call this before iterating the collision set.
	void ClearCollisionSet();

public:

	// Get collision set without validity check. Check validity before calling collisions or iteration.
	TSet<ANaMobSkillCollision*> & GetCollisionSet_Unsafe();

	// Clear invalid elements and get collision set. Safe to iterate. 
	TSet<ANaMobSkillCollision*> & GetCollisionSet_Safe();

	// Add collision to set
	void AddCollision(ANaMobSkillCollision* InCol);

	// Remove collision from set
	void RemoveCollision(ANaMobSkillCollision* InCol);

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "OnSkillHit"), Category = "NaPack|MobSystem")
	void ReceiveCollisionHit(ANaMobSkillCollision* SourceCollision, const FHitResult & HitData);
	virtual void ReceiveCollisionHit_Implementation(ANaMobSkillCollision* SourceCollision, const FHitResult & HitData) {};

	// Switch name to open when the skill is destroyed. None = no open.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName EndSwitchName = NAME_None;

	// Switch time seconds when the skill is destroyed. non-positive = infinity.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float EndSwitchTime = 0.1f;

	void Destroyed() override;

};
