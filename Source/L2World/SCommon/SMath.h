// Namespace SMath is a collection of math utilities as pure C++ functions. No blueprint interface since most are implemented in kismet libraries.
// Here S is not Slate but Sodium-Studio.

#pragma once
#include "CoreMinimal.h"


namespace SMath {

	float Clamp(float value, float min = 0, float max = 1);

	float MoveTo(float from, float to, float max_step_length);

	bool NearlyEqual(float a, float b, float toleration = KINDA_SMALL_NUMBER);
}