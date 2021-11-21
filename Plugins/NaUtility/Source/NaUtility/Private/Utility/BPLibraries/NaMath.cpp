#pragma once
#include "Utility/BPLibraries/NaMath.h"

float UNaMath::Clamp(float value, float min, float max) {
	if (min > max) {
		float temp = max;
		max = min;
		min = temp;
	}
		
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
	return value;
}

float UNaMath::YawClamp(float Yaw) {
	
	Yaw -= float((int((Yaw + 180.f) / 360.f)))*360.f;
	
	while (Yaw > 180.f)
		Yaw -= 180.f;
	while (Yaw <= 180.f)
		Yaw += 180.f;
	return Yaw;
}

float UNaMath::AngleClamp(float Angle) {
	Angle = UNaMath::YawClamp(Angle);
	Angle = abs(Angle);
	return Angle;
}

float UNaMath::MoveTo(float from, float to, float max_step_length) {
	if (from < to) {
		from += max_step_length;
		if (from > to)
			from = to;
	}
	else {
		from -= max_step_length;
		if (from < to)
			from = to;
	}
	return from;
}

float UNaMath::AngularMoveTo_Deg(float from, float to, float max_step_length) {
	float res;
	if (abs(from - to) <= 180)
		res = UNaMath::MoveTo(from, to, max_step_length);
	else if (from < to)
		res = UNaMath::MoveTo(from, to - 360, max_step_length);
	else
		res = UNaMath::MoveTo(from, to + 360, max_step_length);
	UNaMath::YawClamp(res);
	return res;		
}

bool UNaMath::NearlyEqual(float a, float b, float toleration) {
	return (abs(a - b) <= toleration);
}