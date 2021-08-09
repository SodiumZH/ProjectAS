#pragma once
#include "NaMath.h"

float NaMath::Clamp(float value, float min, float max) {
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

float NaMath::YawClamp(float Yaw) {
	
	Yaw -= float((int((Yaw + 180.f) / 360.f)))*360.f;
	
	while (Yaw > 180.f)
		Yaw -= 180.f;
	while (Yaw <= 180.f)
		Yaw += 180.f;
	return Yaw;
}

float NaMath::AngleClamp(float Angle) {
	Angle = NaMath::YawClamp(Angle);
	Angle = abs(Angle);
	return Angle;
}

float NaMath::MoveTo(float from, float to, float max_step_length) {
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

float NaMath::AngularMoveTo_Deg(float from, float to, float max_step_length) {
	float res;
	if (from - to <= 180)
		res = NaMath::MoveTo(from, to, max_step_length);
	else if (from > to)
		res = NaMath::MoveTo(from - 360, to, max_step_length);
	else
		res = NaMath::MoveTo(from, to + 360, max_step_length);
	NaMath::YawClamp(res);
	return res;		
}

bool NaMath::NearlyEqual(float a, float b, float toleration) {
	return (abs(a - b) <= toleration);
}