#pragma once

#include "CoreMinimal.h"
#include "../../NaPack/NaUtility/NaUtility.h"
#include "LCombatData.generated.h"

/* Basic */
// Mob's basic combat data
USTRUCT(BlueprintType)
struct FLMobCombatBasicData {


	GENERATED_USTRUCT_BODY()

public:

	FLMobCombatData() {};

	// Physical basic attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "PhysicalAttack")
		int64 PhysAtk = 1;

	// Physical basic defence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "PhysicalDefence")
		int64 PhysDef = 1;

	// Physical critical, higher => higher physical critical chance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "PhysicalCritial")
		int64 PhysCrit = 0;

	// Physical anticritical, higher => lower chance to take physical critical damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "PhysicalAnticritial")
		int64 PhysAcri = 0;

	// Magical basic attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "MagicalAttack")
		int64 MagAtk = 1;

	// Magical basic defence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "MagicalDefence")
		int64 MagDef = 1;

	// Magical critical, higher => higher magical critical chance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "MagicalCritial")
		int64 MagCrit = 0;

	// Magical anticritical, higher => lower chance to take magical critical damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "MagicalAnticritial")
		int64 MagAcri = 0;

};

/* Element */

// Elements in L2
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

typedef FLElementValue TMap<ELCombatElement, int64>
// Mob's element attack data
USTRUCT(BlueprintType)
struct FLMobElementAtk {


	GENERATED_USTRUCT_BODY()

protected:

	FLElementValue Value= [
	{ ELCombatElement::Fire, 0 },
	{ ELCombatElement::Water, 0 },
	{ ELCombatElement::Earth, 0 },
	{ ELCombatElement::Wind, 0 },
	{ ELCombatElement::Holy, 0 },
	{ ELCombatElement::Dark, 0 },
	{ ELCombatElement::None, 0 }	// No real meaning, just for avoiding undefined key error
	];

public:

	FLMobElementData() {};

	FLMobElementData(int64 Fire = 0, int64 Water = 0, int64 Earth = 0, int64 Wind = 0, int64 Holy = 0, int64 Dark = 0):
		Value([
	{ ELCombatElement::Fire, Fire},
	{ ELCombatElement::Water, Water },
	{ ELCombatElement::Earth, Earth },
	{ ELCombatElement::Wind, Wind },
	{ ELCombatElement::Holy, Holy },
	{ ELCombatElement::Dark, Dark },
	{ ELCombatElement::None, 0 }	// No real meaning, just for avoiding undefined key error
	]);
	{};

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

	int64 GetValue(ELCombatElement Element) { 
		if (Element != ELCombatElement::LElem_None) {
			return Value[Element];
		}
		else {
			LogError("Invalid key: \"None\"");
			return 0;
			}
		};
	int64 SetValue(ELCombatElement Element, int64 Val) {
		if (Element != ELCombatElement::LElem_None)
			Value[Element] = Val;
		else LogError("Invalid key: \"None\"");
	}
}

// Mob's element defence data
USTRUCT(BlueprintType)
struct FLMobElementDef {


	GENERATED_USTRUCT_BODY()

public:

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

typedef FLAtkElementMap TMap<ELCombatElement, bool>;

USTRUCT(BlueprintType)
struct FLAtkElement {

	GENERATED_USTRUCT_BODY()

protected:

	FLAtkElementMap ElementMap = [
	{ ELCombatElement::Fire, false },
	{ ELCombatElement::Water, false },
	{ ELCombatElement::Earth, false },
	{ ELCombatElement::Wind, false },
	{ ELCombatElement::Holy, false },
	{ ELCombatElement::Dark, false },
	{ ELCombatElement::None, false }	// No real meaning, just for avoiding undefined key error
	];

public:

	// No element
	FLAtkElement() {};

	// Single element; None => no element
	FLAtkElement(ELCombatElement Element);

	static FLAtkElement NoElement;
	static FLAtkElement FireElement;
	static FLAtkElement WaterElement;
	static FLAtkElement EarthElement;
	static FLAtkElement WindElement;
	static FLAtkElement HolyElement;
	static FLAtkElement DarkElement;

	// Get the whole element existance map
	FLAtkElementMap & GetMap() { return ElementMap };

	// Get existance of one element
	bool GetElemValue(ELCombatElement Element) { return ElementMap[Element]; };

	// Get how many elements exist
	int ElementCount();

	// Set existance of one element
	void SetElemValue(ELCombatElement Element, bool value) { if (Element != FLCombatElement::None) ElementMap[Element] = value; };

	// Get double-element atk
	void DoubleElement(FLAtkElement Elem1, FLAtkElement Elem2);

};