#pragma once

#include "CoreMinimal.h"
#include "LMobData.generated.h"

/* Mob Race Classes */
UENUM(BlueprintType)
enum class ELMobRace :uint8 {
	// Default race that will not display anything on UI.
	LRace_None	UMETA(DisplayName = "None"),	// 默认

	// Races of controllable characters. Also available on NPCs/monsters.
	LRace_Human	UMETA(DisplayName = "Human"), 	// 人类
	LRace_Elf	UMETA(DisplayName = "Elf"),		// 精灵
	LRace_DElf	UMETA(DisplayName = "DarkElf"),	// 暗精灵
	LRace_Dwarf	UMETA(DisplayName = "Dwarf"),	// 矮人
	LRace_Orc	UMETA(DisplayName = "Orc"),		// 半兽人
	LRace_Kamael	UMETA(DisplayName = "Kamael"),	// 暗天使
	LRace_Ertheia	UMETA(DisplayName = "Ertheia"),	// 翼人

	// Races applied only on NPCs and monsters.
	LRace_Animal	UMETA(DisplayName = "Animal"),	// 动物
	LRace_Monster	UMETA(DisplayName = "Monster"),		// 怪兽
	LRace_Element	UMETA(DisplayName = "Element"),		// 元素
	LRace_Fairy	UMETA(DisplayName = "Fairy"),	// 妖精
	LRace_Undead	UMETA(DisplayName = "Undead"),	// 不死族
	LRace_Humanoid	UMETA(DisplayName = "Humanoid"),	// 类人
	LRace_Machine	UMETA(DisplayName = "Machine"),		// 机械
	LRace_Demon	UMETA(DisplayName = "Demon"),	// 恶魔
	LRace_Angel	UMETA(DisplayName = "Angel"),	// 天使

	LRace_Custom	UMETA(DisplayName = "Custom")	// Manually-shown race 手动设置显示
};

UENUM(BlueprintType)
enum class ELCombatElement :uint8 {
	LElem_None	UMETA(DisplayName = "None"),
	LElem_Fire	UMETA(DisplayName = "Fire"),
	LElem_Water	UMETA(DisplayName = "Water"),
	LElem_Earth	UMETA(DisplayName = "Earth"),
	LElem_Wind	UMETA(DisplayName = "Wind"),
	LElem_Holy	UMETA(DisplayName = "Holyness"),
	LElem_Dark	UMETA(DisplayName = "Darkness")
};


USTRUCT(BlueprintType)
struct FLMobElementData {


	GENERATED_USTRUCT_BODY()

public:

	FLMobElementData() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "FireAttack")
	int64 FireAtk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "WaterAttack")
	int64 WaterAtk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "EarthAttack")
	int64 EarthAtk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "WindAttack")
	int64 WindAtk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "HolynessAttack")
	int64 HolyAtk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "DarknessAttack")
	int64 DarkAtk = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "FireDefence")
	int64 FireDef = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "WaterDefence")
	int64 WaterDef = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "EarthDefence")
	int64 EarthDef = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "WindDefence")
	int64 WindDef = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "HolynessDefence")
	int64 HolyDef = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "DarknessDefence")
	int64 DarkDef = 0;


};

USTRUCT(BlueprintType)
struct FLMobCombatData {


	GENERATED_USTRUCT_BODY()

public:

	FLMobCombatData() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="PhysicalAttack")
	int64 PhysAtk = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "PhysicalDefence")
	int64 PhysDef = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "MagicalAttack")
	int64 MagAtk = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "MagicalDefence")
	int64 MagDef = 1;

//	

};

USTRUCT(BlueprintType)
struct FLMobGeneralData {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELMobRace Race = ELMobRace::LRace_None;

};