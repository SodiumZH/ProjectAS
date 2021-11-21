#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaDebugUtility.generated.h"

// This is a function library for C++ debugging




/* Debug Macros */
#define LogWarning(text) UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(this), TEXT(text))
#define LogError(text) UE_LOG(LogTemp, Error, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(this), TEXT(text))
#define LogWrite(text) UE_LOG(LogTemp, Log, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(this), TEXT(text))

#define LogWarningContext(text, context) UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(context), TEXT(text))
#define LogErrorContext(text, context) UE_LOG(LogTemp, Error, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(context), TEXT(text))
#define LogWriteContext(text, context) UE_LOG(LogTemp, Log, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(context), TEXT(text))

#define LogWarningNoContext(text) UE_LOG(LogTemp, Warning, TEXT(text))
#define LogErrorNoContext(text) UE_LOG(LogTemp, Error, TEXT(text))
#define LogWriteNoContext(text) UE_LOG(LogTemp, Log, TEXT(text))

#define PrintToScreen(text) UNaDebugUtility::DebugInfo(this, TEXT(text))

UCLASS()
class NAUTILITY_API UNaDebugUtility: public UBlueprintFunctionLibrary {

	GENERATED_BODY()
public:


	// Print debug information with a specific object source
	static void DebugInfo(UObject* Source, FString Info, float Time = 2.0f, bool bShowSource = true, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor Color = FLinearColor(0.f, 0.66f, 1.f));

	// Get display name
	static FString DisplayName(UObject* InObj);



};

