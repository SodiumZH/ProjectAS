#pragma once

#include "LCombatData.h"
#include "LCombatRule.h"

/* Element Value */

FLElementValue::FLElementValue() {

	Value.Emplace(ELElement::LElem_Fire, 0);
	Value.Emplace(ELElement::LElem_Water, 0);
	Value.Emplace(ELElement::LElem_Earth, 0);
	Value.Emplace(ELElement::LElem_Wind, 0);
	Value.Emplace(ELElement::LElem_Holy, 0);
	Value.Emplace(ELElement::LElem_Dark, 0);
	Value.Emplace(ELElement::LElem_None, 0); // No real meaning, just for avoiding undefined key error
	
};

FLElementValue FLElementValue::Make(int64 Fire, int64 Water, int64 Earth, int64 Wind, int64 Holy, int64 Dark) {
	FLElementValue Out = FLElementValue();
	Out.Value.Emplace(ELElement::LElem_Fire, Fire);
	Out.Value.Emplace(ELElement::LElem_Water, Water);
	Out.Value.Emplace(ELElement::LElem_Earth, Earth);
	Out.Value.Emplace(ELElement::LElem_Wind, Wind);
	Out.Value.Emplace(ELElement::LElem_Holy, Holy);
	Out.Value.Emplace(ELElement::LElem_Dark, Dark);
	return Out;
}

int64 FLElementValue::Get(ELElement Element) {
	if (Element != ELElement::LElem_None) {
		return Value[Element];
	}
	else {
		L_ELEMENT_NONE_ERROR;
		return 0;
	}
}

void FLElementValue::Set(ELElement Element, int64 Val) {
	if (Element != ELElement::LElem_None)
		Value[Element] = Val;
	else L_ELEMENT_NONE_ERROR;
}

/* Element Type */

FLElementType::FLElementType(){
	Map.Emplace(ELElement::LElem_Fire, false);
	Map.Emplace(ELElement::LElem_Water, false);
	Map.Emplace(ELElement::LElem_Earth, false);
	Map.Emplace(ELElement::LElem_Wind, false);
	Map.Emplace(ELElement::LElem_Holy, false);
	Map.Emplace(ELElement::LElem_Dark, false);
	Map.Emplace(ELElement::LElem_None, false); // No real meaning, just for avoiding undefined key error
}
FLElementType::FLElementType(ELElement Element) {
	Map.Emplace(ELElement::LElem_Fire, false);
	Map.Emplace(ELElement::LElem_Water, false);
	Map.Emplace(ELElement::LElem_Earth, false);
	Map.Emplace(ELElement::LElem_Wind, false);
	Map.Emplace(ELElement::LElem_Holy, false);
	Map.Emplace(ELElement::LElem_Dark, false);
	Map.Emplace(ELElement::LElem_None, false); // No real meaning, just for avoiding undefined key error
	if (Element != ELElement::LElem_None)
		Map[Element] = true;
}

bool FLElementType::IsIn(ELElement Element) {
	if (Element != ELElement::LElem_None)
		return Map[Element];
	else {
		L_ELEMENT_NONE_ERROR;
		return false;
	}
}

int FLElementType::Count() {
	int Out = 0;
	for (auto & pair : Map) {
		if ((pair.Key != ELElement::LElem_None) && (pair.Value))
			Out++;
	}
	return Out;
}

void FLElementType::Set(ELElement Element, bool value){
	if (Element != ELElement::LElem_None) 
		Map[Element] = value; 
	else 
		L_ELEMENT_NONE_ERROR;
}

FLElementType FLElementType::Double(ELElement Elem1, ELElement Elem2) {
	FLElementType Out = FLElementType(Elem1);
	Out.Set(Elem2, true);
	return Out;
}

FLElementType FLElementType::None = FLElementType();
FLElementType FLElementType::Fire = FLElementType(ELElement::LElem_Fire);
FLElementType FLElementType::Water = FLElementType(ELElement::LElem_Water);
FLElementType FLElementType::Earth = FLElementType(ELElement::LElem_Earth);
FLElementType FLElementType::Wind = FLElementType(ELElement::LElem_Wind);
FLElementType FLElementType::Holy = FLElementType(ELElement::LElem_Holy);
FLElementType FLElementType::Dark = FLElementType(ELElement::LElem_Dark);


