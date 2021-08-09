#pragma once

#include "CoreMinimal.h"


namespace NaString {

	FString ToString(int source);
	FString ToString(float source);
	FString ToString(FVector source);
	FString ToString(FRotator source);
	FString ToString(bool source);

};