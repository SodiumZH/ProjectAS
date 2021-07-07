#pragma once

#include "CoreMinimal.h"

/* Mob Race Classes */
UENUM(BlueprintType)
enum class ELRace :uint8 {
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