#pragma once
#include "SMath.h"

float SMath::Clamp(float value, float min, float max) {
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

float SMath::MoveTo(float from, float to, float max_step_length) {
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

bool NearlyEqual(float a, float b, float toleration) {
	return (abs(a - b) <= toleration);
}