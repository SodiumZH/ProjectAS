#pragma once

#include "CoreMinimal.h"
#include "LMobData.generated.h"



/* Mob Race Classes */
UENUM(BlueprintType)
enum class ELRace {
	// Default race that will not display anything on UI.
	Race_None	UMETA(DisplayName = "None"),	// 默认

	// Races of controllable characters. Also available on NPCs/monsters.
	Race_Human	UMETA(DisplayName = "Human"), 	// 人类
	Race_Elf	UMETA(DisplayName = "Elf"),		// 精灵
	Race_DElf	UMETA(DisplayName = "DarkElf"),	// 暗精灵
	Race_Dwarf	UMETA(DisplayName = "Dwarf"),	// 矮人
	Race_Orc	UMETA(DisplayName = "Orc"),		// 半兽人
	Race_Kamael	UMETA(DisplayName = "Kamael"),	// 暗天使
	Race_Ertheia	UMETA(DisplayName = "Ertheia")	// 翼人

	// Races applied only on NPCs and monsters.
	Race_Animal	UMETA(DisplayName = "Animal"),	// 动物
	Race_Monster	UMETA(DisplayName = "Monster"),		// 怪兽
	Race_Element	UMETA(DisplayName = "Element"),		// 元素
	Race_Fairy	UMETA(DisplayName = "Fairy"),	// 妖精
	Race_Undead	UMETA(DisplayName = "Undead"),	// 不死族
	Race_Humanoid	UMETA(DisplayName = "Humanoid"),	// 类人
	Race_Machine	UMETA(DisplayName = "Machine"),		// 机械
	Race_Demon	UMETA(DisplayName = "Demon"),	// 恶魔
	Race_Angel	UMETA(DisplayName = "Angel"),	// 天使
};

/* */


/**
* Public status data for all mobs
*/
USTRUCT(BlueprintType)
struct FLMobPublicData {

	GENERATED_USTRUCT_BODY()

public: 

	FLMobStatus() {};

	/* Basic Information */

	// Mob's display name in UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = TEXT("Mob");

	// Mob's race
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELRace Race = ELRace::Race_None;




	// Current health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 CurrentHP = 100;

	// Current magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 CurrentMP = 100;

	// Max health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 MaxHP = 100;

	// Max magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 MaxMP = 100;

	// Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 Atk = 1;

	// Defense
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 Def = 1;

};

/**
* Data valid only on players
*/
USTRUCT(BlueprintType)
struct FLPlayerStatus {

	GENERATED_USTRUCT_BODY()


}