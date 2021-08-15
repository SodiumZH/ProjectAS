#pragma once

#include "LCombatRule.h"

double ULCombatRule::GetBasicDamage(const FLSkillAtk & SkillAtk, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData) {
	int64 Atk = 0;
	int64 Def = 0;
	switch (SkillAtk.Type) {
	case ELAtkType::Atk_Phys: {
		Atk = SourceData.PhysAtk;
		Def = TargetData.PhysDef;
		break;
	}
	case ELAtkType::Atk_Mag: {
		Atk = SourceData.MagAtk;
		Def = TargetData.MagDef;
		break;
	}
	default: {
		checkNoEntry();
	}
	}
	if (SkillAtk.bIgnoreDefence)
		Def = 0;

	double DamageRate = (double)Atk / ((double)Def + BASIC_DEFENCE_CONST);
	return DamageRate*SkillAtk.BasicValue*BASIC_DAMAGE_CONST*FMath::RandRange(1.0 - DAMAGE_FLUCTUATE_RATE, 1.0 + DAMAGE_FLUCTUATE_RATE);
}

float ULCombatRule::GetCritRate(const FLSkillAtk & SkillAtk, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData) {
	int64 Crit = 0;
	int64 Acri = 0;

	switch (SkillAtk.Type) {
	case ELAtkType::Atk_Phys: {
		Crit = SourceData.PhysCrit;
		Acri = SourceData.PhysAcri;
		break;
	}
	case ELAtkType::Atk_Mag: {
		Crit = SourceData.MagCrit;
		Crit = SourceData.MagAcri;
		break;
	}
	default: {
		checkNoEntry();
	}
	}

	return FMath::Clamp(((float)Crit / (float)(Acri + BASIC_ANTICRIT)) / CRIT_MAX, 0, 1);

}

float ULCombatRule::GetElemDamageRate(const FLSkillAtk & SkillAtk, const FLMobCombatBasicData & SourceData, const FLMobCombatBasicData & TargetData) {

}
