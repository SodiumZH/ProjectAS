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

UCLASS(BlueprintType)
class NAPACK_API ANaMobSkill : public AActor 
{

	GENERATED_BODY()

public:

	ANaMobSkill() {};

	class ANaMob* Source;

	FName Socket;




};
