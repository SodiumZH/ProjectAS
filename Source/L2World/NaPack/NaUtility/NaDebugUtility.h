#pragma once

#include "CoreMinimal.h"
#include "../NaGlobalHeader.h"

// This is a function library for C++ debugging

DECLARE_LOG_CATEGORY_EXTERN(LogNaPack, Log, All);

/* Debug Macros */
#if NAPACK_ENABLE_DEBUG
#define LogWarning(text) UE_LOG(LogNaPack, Warning, TEXT("%s: %s"), *LNaDebugUtility::DisplayName(this), TEXT(text))
#define LogError(text) UE_LOG(LogNaPack, Error, TEXT("%s: %s"), *LNaDebugUtility::DisplayName(this), TEXT(text))
#define LogWrite(text) UE_LOG(LogNaPack, Log, TEXT("%s: %s"), *LNaDebugUtility::DisplayName(this), TEXT(text))

#define LogWarningContext(text, context) UE_LOG(LogNaPack, Warning, TEXT("%s: %s"), *LNaDebugUtility::DisplayName(context), TEXT(text))
#define LogErrorContext(text, context) UE_LOG(LogNaPack, Error, TEXT("%s: %s"), *LNaDebugUtility::DisplayName(context), TEXT(text))
#define LogWriteContext(text, context) UE_LOG(LogNaPack, Log, TEXT("%s: %s"), *LNaDebugUtility::DisplayName(context), TEXT(text))

#define LogWarningNoContext(text) UE_LOG(LogNaPack, Warning, TEXT(text))
#define LogErrorNoContext(text) UE_LOG(LogNaPack, Error, TEXT(text))
#define LogWriteNoContext(text) UE_LOG(LogNaPack, Log, TEXT(text))

#define PrintToScreen(text) LNaDebugUtility::DebugInfo(this, TEXT(text))
#else
#define LogWarning(text) 0
#define LogError(text) 0
#define LogWrite() 0

#define LogWarningContext(text, context) 0
#define LogErrorContext(text, context) 0
#define LogWriteContext(text, context) 0

#define LogWarningNoContext(text) 0
#define LogErrorNoContext(text) 0
#define LogWriteNoContext 0

#define PrintToScreen(text) 0
#endif



namespace LNaDebugUtility {

	// Print debug information with a specific object source
	void DebugInfo(UObject* Source, FString Info, float Time = 2.0f, bool bShowSource = true, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor Color = FLinearColor(0.f, 0.66f, 1.f));

	// Get display name
	FString DisplayName(UObject* InObj);

}

