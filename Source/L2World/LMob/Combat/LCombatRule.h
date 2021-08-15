#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LCombatData.h"
#include "LCombatRule.generated.h"


UCLASS()
class L2WORLD_API ULCombatRule : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	float GetPhysBasicDamage(int64 SkillAtk, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData);

	float GetMagBasicDamage(int64 SkillAtk, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData);

	float GetPhysCritRate(const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData);

	float GetMagCritRate(const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData);

	float GetElemDamageRate(ELCombatElement Elem, int64 SkillElem, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData)


};