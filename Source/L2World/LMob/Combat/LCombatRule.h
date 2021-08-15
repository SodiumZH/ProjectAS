#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LCombatData.h"
#include "LCombatRule.generated.h"

class ALMob;

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
	int64 Basic = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELElement Element = ELElement::LElem_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIgnoreDefence = false;
	
};


/*
* Numerical rules for calculating damage
*
*/

/* Definition of constants */
#define BASIC_DAMAGE_CONST 20.0
#define BASIC_DEFENCE_CONST 20.0
#define ELEM_CONST 20.0
#define BASIC_ANTICRIT 20.0
#define CRIT_MAX 100.0
#define DAMAGE_FLUCTUATE_RATE 0.1

UCLASS()
class L2WORLD_API ULCombatRule : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	// Get basic damage of a skill. 
	// 
	UFUNCTION(BlueprintPure, Category="L2|Combat")
	int64 GetBasicDamage(const FLSkillAtk & SkillAtk, const FLMobCombatData & SourceData, const FLMobCombatData & TargetData);

	// Get probability of critical
	UFUNCTION(BlueprintPure, meta=(DisplayName="GetCriticalRate"), Category = "L2|Combat")
	float GetCritRate(const FLSkillAtk & SkillAtk, const FLMobCombatData & SourceData, const FLMobCombatData & TargetData);

	// Damage rate from element. NOT IMPLEMENTED and return 1.0 by default
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetElementDamageRate"), Category = "L2|Combat")
	float GetElemDamageRate(const FLSkillAtk & SkillAtk, ALMob* Source, ALMob* TargetData)

	

};