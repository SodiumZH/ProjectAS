#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LCombatData.h"
#include "LCombatRule.generated.h"

/* Attack type: physical/magical */
UENUM(BlueprintType)
enum class ELAtkType :uint8 {
	Atk_Phys	UMETA(DisplayName = "Physical"),
	Atk_Mag		UMETA(DisplayName = "Magical")
};


/* Skill's attack data */
USTRUCT(BlueprintType)
struct FLSkillAtk {

	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELAtkType Type = ELAtkType::Atk_Phys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 BasicValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELCombatElement Element = ELCombatElement::LElem_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIgnoreDefence = false;
	
};


/*
* Numerical rules for calculating damage
*
*/

#define BASIC_DAMAGE_CONST 20.0
#define BASIC_DEFENCE_CONST 20.0
#define ELEM_CONST 20.0
#define BASIC_ANTICRIT 20.0
#define CRIT_MAX 200.0
#define DAMAGE_FLUCTUATE_RATE 0.1

UCLASS()
class L2WORLD_API ULCombatRule : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:



	int64 GetBasicDamage(const FLSkillAtk & SkillAtk, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData);

	float GetCritRate(const FLSkillAtk & SkillAtk, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData);


	float GetElemDamageRate(const FLSkillAtk & SkillAtk, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData)


};