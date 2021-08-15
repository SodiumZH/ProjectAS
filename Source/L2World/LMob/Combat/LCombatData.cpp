#pragma once

#include "LCombatRule.h"

/* Attack element */

FLAtkElement::FLAtkElement(ELCombatElement Element) {
	ElementMap = [
	{ ELCombatElement::Fire, false },
	{ ELCombatElement::Water, false },
	{ ELCombatElement::Earth, false },
	{ ELCombatElement::Wind, false },
	{ ELCombatElement::Holy, false },
	{ ELCombatElement::Dark, false },
	{ ELCombatElement::None, false }
	];
	if (Element != ELCombatElement::None)
		ElementMap[Element] = true;
}

int FLAtkElement::ElementCount() {
	int Out = 0;
	for (auto & pair : ElementMap) {
		if ((pair.Key != ELCombatElement::None) && (pair.Value))
			Out++;
	}
	return Out;
}

FLAtkElement FLAtkElement::NoElement = FLAtkElement();
FLAtkElement FLAtkElement::FireElement = FLAtkElement(ELCombatElement::Fire);
FLAtkElement FLAtkElement::WaterElement = FLAtkElement(ELCombatElement::Water);
FLAtkElement FLAtkElement::EarthElement = FLAtkElement(ELCombatElement::Earth);
FLAtkElement FLAtkElement::WindElement = FLAtkElement(ELCombatElement::Wind);
FLAtkElement FLAtkElement::HolyElement = FLAtkElement(ELCombatElement::Holy);
FLAtkElement FLAtkElement::DarkElement = FLAtkElement(ELCombatElement::Dark);

void FLAtkElement::DoubleElement(FLAtkElement Elem1, FLAtkElement Elem2) {
	FLAtkElement Out = FLAtkElement(Elem1);
	Out.SetElement(Elem2, true);
	return Out;
};
