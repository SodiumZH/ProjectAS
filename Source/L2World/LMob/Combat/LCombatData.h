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
enum class ELElement :uint8 {
	LElem_None	UMETA(DisplayName = "None"),
	LElem_Fire	UMETA(DisplayName = "Fire"),
	LElem_Water	UMETA(DisplayName = "Water"),
	LElem_Earth	UMETA(DisplayName = "Earth"),
	LElem_Wind	UMETA(DisplayName = "Wind"),
	LElem_Holy	UMETA(DisplayName = "Holyness"),
	LElem_Dark	UMETA(DisplayName = "Darkness")
};


// Mob's element numerical data
typedef TMap<ELElement, int64> FLElementValueMap;
#define L_ELEMENT_NONE_ERROR LogError("Invalid element key: \"None\"")
USTRUCT(BlueprintType)
struct FLElementValue {

	GENERATED_USTRUCT_BODY()

protected:

	FLElementValueMap Value= [
	{ ELElement::LElem_Fire, 0 },
	{ ELElement::LElem_Water, 0 },
	{ ELElement::LElem_Earth, 0 },
	{ ELElement::LElem_Wind, 0 },
	{ ELElement::LElem_Holy, 0 },
	{ ELElement::LElem_Dark, 0 },
	{ ELElement::LElem_None, 0 }	// No real meaning, just for avoiding undefined key error
	];

public:

	FLMobElementAtk() {};

	FLMobElementAtk(int64 Fire = 0, int64 Water = 0, int64 Earth = 0, int64 Wind = 0, int64 Holy = 0, int64 Dark = 0):
		Value([
	{ ELElement::LElem_Fire, Fire},
	{ ELElement::LElem_Water, Water },
	{ ELElement::LElem_Earth, Earth },
	{ ELElement::LElem_Wind, Wind },
	{ ELElement::LElem_Holy, Holy },
	{ ELElement::LElem_Dark, Dark },
	{ ELElement::LElem_None, 0 }	// No real meaning, just for avoiding undefined key error
	])
	{};

	FLElementValueMap GetValue() { return Value; };

	int64 Get(ELElement Element) { 
		if (Element != ELElement::LElem_None) {
			return Value[Element];
		}
		else {
			L_ELEMENT_NONE_ERROR;
			return 0;
			}
		};
	int64 Set(ELElement Element, int64 Val) {
		if (Element != ELElement::LElem_None)
			Value[Element] = Val;
		else L_ELEMENT_NONE_ERROR;
	}

	int64 operator[](ELElement Elem) { return Get(Elem); };
}


// Struct to show whether an attack is in elements
typedef TMap<ELElement, bool> FLElementTypeMap;
USTRUCT(BlueprintType)
struct FLElementType {

	GENERATED_USTRUCT_BODY()

protected:

	FLElementTypeMap Map = [
	{ ELElement::LElem_Fire, false },
	{ ELElement::LElem_Water, false },
	{ ELElement::LElem_Earth, false },
	{ ELElement::LElem_Wind, false },
	{ ELElement::LElem_Holy, false },
	{ ELElement::LElem_Dark, false },
	{ ELElement::LElem_None, false }	// No real meaning, just for avoiding undefined key error.
	];

public:

	// No element
	FLElementType() {};

	// Single element; None => no element
	FLElementType(ELElement Element);

	static FLElementType None;
	static FLElementType Fire;
	static FLElementType Water;
	static FLElementType Earth;
	static FLElementType Wind;
	static FLElementType Holy;
	static FLElementType Dark;

	// Get the whole element existance map
	FLElementTypeMap & GetValue() { return Map };

	// Get existance of one element
	bool IsIn(ELElement Element);
	bool Get(ELElement Element) { return IsIn(Element); };
	bool operator[](ELElement Elem) { return IsIn(Elem); };

	// Get how many elements exist
	int Count();

	// Set existance of one element
	void Set(ELElement Element, bool value);

	// Get double-element atk
	void Double(ELElement Elem1, ELElement Elem2);

};


/* Overall */

// Overall combat data for mobs
USTRUCT(BlueprintType)
struct FLMobCombatData {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLMobCombatBasicData Basic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "ElementAttack")
	FLElementValue ElemAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "ElementDefence")
	FLElementValue ElemDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "BasicAttackElement")
	FLElementType BasicAtkElem;





};