#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logging/LogMacros.h"
#include "NaDebugUtility.generated.h"

/* Natrium logs */
NAUTILITY_API DECLARE_LOG_CATEGORY_EXTERN(LogNaUtil, Log, All);
NAUTILITY_API DECLARE_LOG_CATEGORY_EXTERN(LogNaTemp, Log, All);


/* Input checking verbosity */

/* If true, functions will do many extra checks for inputs and object states to ensure robustness.
* Setting this value false may improve performance, but may increase hazards if unexpected cases occur.
* For example, checking codes under this macro may catch an input error and only print to log, or it will cause a crash.
* When disabled, some errors may appear as asserts which crashes the program without enough information, instead of a handled actions with a single error information.
*/
#define NAPACK_DO_VERBOSE_CHECK true

/* If true, functions will do common checks to prevent serious problems like overflows and undefined actions.
* Setting this value false may improve performance but is very unsafe, so not recommended.
* It's recommended to keep this value true in the published version.
*/
#define NAPACK_DO_COMMON_CHECK true


// This is a function library for C++ debugging

#define UE_LOG_UOBJECT(SourceUObject, Category, Verbosity, Info) UE_LOG(Category, Verbosity, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(SourceUObject), TEXT(Info))

/* Debug Macros */
/* Deprecated, should be replaced by UE_LOG_UOBJECT */
#define LogWarning(text) UE_LOG(LogNaTemp, Warning, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(this), TEXT(text))
#define LogError(text) UE_LOG(LogNaTemp, Error, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(this), TEXT(text))
#define LogWrite(text) UE_LOG(LogNaTemp, Display, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(this), TEXT(text))

#define LogWarningContext(text, context) UE_LOG(LogNaTemp, Warning, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(context), TEXT(text))
#define LogErrorContext(text, context) UE_LOG(LogNaTemp, Error, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(context), TEXT(text))
#define LogWriteContext(text, context) UE_LOG(LogNaTemp, Display, TEXT("%s: %s"), *UNaDebugUtility::DisplayName(context), TEXT(text))

#define LogWarningNoContext(text) UE_LOG(LogNaTemp, Warning, TEXT(text))
#define LogErrorNoContext(text) UE_LOG(LogNaTemp, Error, TEXT(text))
#define LogWriteNoContext(text) UE_LOG(LogNaTemp, Display, TEXT(text))

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

