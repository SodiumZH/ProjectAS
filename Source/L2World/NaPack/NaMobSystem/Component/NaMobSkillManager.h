/* By Sodium */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../NaGlobalHeader.h"
#include "../../NaUtility/NaUtility.h"
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

UCLASS(BlueprintType)
class NAPACK_API UNaMobSkillManager : public UActorComponent {
	
	GENERATED_BODY()

protected:

	TMap<FName, ANaMobSkill*> SkillRegistry;

public:

	UNaMobSkillManager();

	/* Skill Registry R&W */

	void RegisterSkill(FName RegisterName, ANaMobSkill* InSkill);

	void UnregisterSkill(FName RegisterName);

	void UnregisterSkill(ANaMobSkill* InSkill);

	ANaMobSkill* GetSkillFromRegisterName(FName RegisterName);

	FName GetSkillRegisterName(ANaMobSkill* Skill);

	ANaMobSkill* UseSkill(
		TSubclassOf<ANaMobSkill> SkillClass,
		const FTransform & InTransform,
		FName InRegisterName,
		class USceneComponent* AttachToComponent = nullptr,
		FName SocketName = NAME_None,
		bool DoAttachment = true
	);

	
};