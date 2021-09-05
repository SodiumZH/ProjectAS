#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../NaGlobalHeader.h"
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

DECLARE_DYNAMIC_DELEGATE(FNaMobSkillTimelineEvent);

UCLASS(BlueprintType)
class NAPACK_API ANaMobSkill : public AActor 
{

	GENERATED_BODY()

public:

	ANaMobSkill() {};

	virtual void OnConstruction(const FTransform & Trans) override;

	virtual void Tick(float DeltaTime) override;

	/* Installation */

	UPROPERTY(BlueprintReadOnly)
	class ANaMob* Source;

	UPROPERTY(BlueprintReadOnly)
	FName Socket;

	/** Generate a skill object from a mob.
	* @Param SourceMob The mob as source of this skill.
	* @Param SkillClass Applied skill class.
	* @Param InTransform Relative transform.
	* @Param SocketName Socket of attachment of this skill.
	* @Param AttachToComponent Component of actor this skill should attach. If this param is left null, skill will attach to the root component.
	* @Param AttachToActor Actor this skill should attach. If this param is left null, skill will attach to the source mob.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaPack|NaMobSystem")
	static ANaMobSkill* UseSkillByClass(ANaMob* SourceMob, TSubclassOf<ANaMobSkill> SkillClass, const FTransform & InTransform, FName SocketName = NAME_None, USceneComponent* AttachToComponent=nullptr, AActor* AttachToActor=nullptr);

	/* Timeline */


	

	TMultiMap<float, FNaMobSkillTimelineEvent> TimelineMap;

	void AddTimepointEvent(float Time, void(UObject::*Event)(void));
	void AddTimepointEvent(float Time, void(UObject::*Event)(void), UObject* Target);

	UFUNCTION(BlueprintCallable, Category = "NaPack|NaMobSystem")
	void AddTimepointEvent_Delegate(float Time, FNaMobSkillTimelineEvent Event);

protected:

	float StartTime = -1; // -1 means uninitialized
	float TimeNow = -1;
	float TimeLastTick = -1;

	// Get time in seconds from skill spawning
	UFUNCTION(BlueprintCallable, Category = "NaPack|NaMobSystem")
	float GetTime();

	// Get time in seconds from skill spawning to last tick
	float GetTimeLastTick();

	void Tick_Timeline();


};
