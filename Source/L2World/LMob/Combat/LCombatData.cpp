#pragma once

#include "LCombatRule.h"




/* Element Type */

FLElementType::FLElementType(ELElement Element) {
	ElementMap = [
	{ ELElement::LElem_Fire, false },
	{ ELElement::LElem_Water, false },
	{ ELElement::LElem_Earth, false },
	{ ELElement::LElem_Wind, false },
	{ ELElement::LElem_Holy, false },
	{ ELElement::LElem_Dark, false },
	{ ELElement::LElem_None, false }
	];
	if (Element != ELElement::None)
		ElementMap[Element] = true;
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
	for (auto & pair : ElementMap) {
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

void FLElementType::Double(ELElement Elem1, ELElement Elem2) {
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


