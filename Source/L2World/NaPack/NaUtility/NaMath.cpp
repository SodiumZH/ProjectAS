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

bool NaMath::NearlyEqual(float a, float b, float toleration) {
	return (abs(a - b) <= toleration);
}