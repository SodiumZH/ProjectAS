#pragma once

#include "NaSystem.h"

template<typename T>
int NaSystem::GetDataSize(T Data) {
	return sizeof(Data);
}

template<typename T>
int NaSystem::GetObjectSize(T* ObjPtr) {
	return sizeof(*ObjPtr);
}