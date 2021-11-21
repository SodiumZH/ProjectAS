/* By Sodium */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaUtility.h"
#include "NaMobSkillManager.generated.h"

/* NaMob Skill Manager is an interface component attached only to NaMob for handling skills (based on class ANaMobSkill). 
* 
* It manages used skill with a registry which maps every skill with a unique register name. Warning: "None" represents null and is not a valid register name.
* This component also do actions of using skill, and register at the same time. 
* All existing skill of its owner mob should be registered with a unique name, while destroyed skill should be unregistered.
* For details, see documentation.
* 
*/

class ANaMobSkill;
struct FMobSkillUsageOptions;

UCLASS(BlueprintType)
class UNaMobSkillManager : public UActorComponent {
	
	GENERATED_BODY()

protected:

	TMap<FName, ANaMobSkill*> SkillRegistry;

public:

	UNaMobSkillManager();

	/* Skill Registry R&W */

	TArray<ANaMobSkill*> GetAllSkills();

	TArray<FName> GetAllRegisterNames();

	// Return if a register name exists
	bool ContainsRegisterName(FName InName);

	/* Register a skill in skill manager.
	* @Param bForce If true, when the register name exists, the new skill will overwrite the old one.
	*/
	bool RegisterSkill(FName RegisterName, ANaMobSkill* InSkill, bool bForce = false);

	void UnregisterSkill(FName RegisterName);

	void UnregisterSkill(ANaMobSkill* InSkill);

	ANaMobSkill* GetSkillFromRegisterName(FName RegisterName);

	FName GetSkillRegisterName(ANaMobSkill* Skill);

	void DestroySkillFromName(FName Name);

	ANaMobSkill* UseSkill(
		TSubclassOf<ANaMobSkill> SkillClass,
		const FTransform & InTransform,
		FName InRegisterName,
		const FMobSkillUsageOptions & Options,
		bool Force = false,
		class USceneComponent* AttachToComponent = nullptr,
		FName SocketName = NAME_None
	);

	
};