#pragma once

#include "CoreMinimal.h"
#include "LMobData.generated.h"



/* Mob Race Classes */
UENUM(BlueprintType)
enum class ELRace {
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
	LRace_Angel	UMETA(DisplayName = "Angel")	// 天使

	LRace_Custom	UMETA(DisplayName = "Custom")	// Manually-shown race 手动设置显示
};

/* Mob gender 
* L2 game contains this value, but usage is unknown
*/
UENUM(BlueprintType)
enum class ELGender {
	LGender_None	UMETA(DisplayName = "None"),
	LGender_Male	UMETA(DisplayName = "Male"),
	LGender_Female	UMETA(DisplayName = "Female"),
	LGender_Other	UMETA(DisplayName = "Other"),
	LGender_Custom	UMETA(DisplayName = "Custom Gender")
};


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELGender Gender = ELGender::None;


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


	/* Movement related */

	// Speed on walking
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeedScale = 1.f;

	// Speed on running
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeedScale = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRunning = true;

	// Max acceleration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, UMETA(DisplayName = "Max Acceleration Scale"))
	float MaxAccelScale = 10.f;

	// Enable jump?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanJump = true;

	// Max jump height
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeightScale = 1.f;





};

/**
* Data valid only on players
*/
USTRUCT(BlueprintType)
struct FLPlayerStatus {

	GENERATED_USTRUCT_BODY()
public:

	// Unique ID of player data.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerID = 0;

	
}