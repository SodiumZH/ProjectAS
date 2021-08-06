// Namespace NaMath is a collection of math utilities as pure C++ functions. No blueprint interface since most are implemented in kismet libraries.S

#pragma once
#include "CoreMinimal.h"


namespace NaMath {

	template <typename T>
	T Max(T a, T b) { return a > b ? a : b; };

	template <typename T>
	T Min(T a, T b) { return a > b ? b : a; };


	float Clamp(float value, float min = 0, float max = 1);

	// Clamp a yaw value between -180~180 w/o changing its true value.
	float YawClamp(float Yaw);

	// Clamp an angle between 0~180 w/o changing its true value.
	float AngleClamp(float Angle);

	float MoveTo(float from, float to, float max_step_length);

	float AngularMoveTo_Deg(float from, float to, float max_step_length);

	bool NearlyEqual(float a, float b, float toleration = KINDA_SMALL_NUMBER);
}