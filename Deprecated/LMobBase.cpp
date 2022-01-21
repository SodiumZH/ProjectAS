// Fill out your copyright notice in the Description page of Project Settings.


#include "LMobBase.h"

// Add default functionality here for any ILMobBase functions that are not pure virtual.

ELMobRace ILMobBase::GetRace() {
	return Race;
}

ELMobRace ILMobBase::SetRace(ELMobRace NewRace) {
	Race = NewRace;
	return Race;
}