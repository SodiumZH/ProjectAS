#pragma once

#include "CoreMinimal.h"
#include "../NaGlobalHeader.h"

// This is a function library for C++ debugging

/* Debug Macros */
#if NAPACK_ENABLE_DEBUG
#define LogWarning(text) UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *NaDebugUtility::DisplayName(this), TEXT(text))
#define LogError(text) UE_LOG(LogTemp, Error, TEXT("%s: %s"), *NaDebugUtility::DisplayName(this), TEXT(text))
#else
#define LogWarning(text)
#define LogError(text)
#endif


namespace NaDebugUtility {

	// Print debug information with a specific object source
	void DebugInfo(UObject* Source, FString Info, float Time = 2.0f, bool bShowSource = true, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor Color = FLinearColor(0.f, 0.66f, 1.f));

	// Get display name
	FString DisplayName(UObject* InObj);

}

