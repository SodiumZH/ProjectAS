#pragma once

#include "NaString.h"
#include "Kismet/KismetStringLibrary.h"

FString NaString::ToString(int source) {
	return UKismetStringLibrary::Conv_IntToString(source);
}
FString NaString::ToString(float source) {
	return UKismetStringLibrary::Conv_FloatToString(source);
}
FString NaString::ToString(FVector source) {
	return UKismetStringLibrary::Conv_VectorToString(source);
}
FString NaString::ToString(FRotator source) {
	return UKismetStringLibrary::Conv_RotatorToString(source);
}
FString NaString::ToString(bool source) {
	return UKismetStringLibrary::Conv_BoolToString(source);
}