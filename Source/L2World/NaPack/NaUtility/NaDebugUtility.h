#pragma once

#include "CoreMinimal.h"
#include "../NaGlobalHeader.h"

// This is a function library for C++ debugging

DECLARE_LOG_CATEGORY_EXTERN(LogNaPack, Log, All);

/* Debug Macros */
#if NAPACK_ENABLE_DEBUG
#define LogWarning(text) UE_LOG(LogNaPack, Warning, TEXT("%s: %s"), *NaDebugUtility::DisplayName(this), TEXT(text))
#define LogError(text) UE_LOG(LogNaPack, Error, TEXT("%s: %s"), *NaDebugUtility::DisplayName(this), TEXT(text))
#define LogWarningContext(text, context) UE_LOG(LogNaPack, Warning, TEXT("%s: %s"), *NaDebugUtility::DisplayName(context), TEXT(text))
#define LogErrorContext(text, context) UE_LOG(LogNaPack, Error, TEXT("%s: %s"), *NaDebugUtility::DisplayName(context), TEXT(text))
#define LogWarningNoContext(text) UE_LOG(LogNaPack, Warning, TEXT(text))
#define LogErrorNoContext(text) UE_LOG(LogNaPack, Error, TEXT(text))
#define PrintToScreen(text) NaDebugUtility::DebugInfo(this, TEXT(text))
#else
#define LogWarning(text) 0
#define LogError(text) 0
#define LogWarningNoContext(text) 0
#define LogErrorNoContext(text) 0
#define PrintToScreen(text) 0
#endif



namespace NaDebugUtility {

	// Print debug information with a specific object source
	void DebugInfo(UObject* Source, FString Info, float Time = 2.0f, bool bShowSource = true, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor Color = FLinearColor(0.f, 0.66f, 1.f));

	// Get display name
	FString DisplayName(UObject* InObj);

}

