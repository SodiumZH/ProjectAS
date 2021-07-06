#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "L2World.h"
#include "LUtil.generated.h"

// Simplified macros for debug displaying
#define DebugInfo(text) ULUTil::LPrintDebugInformation(this, TEXT(text))
#define DebugInfoDt(text, dt) ULUTil::LPrintDebugInformation(this, TEXT(text), dt)
#define DebugInfoStr(str) ULUTil::LPrintDebugInformation(this, str)
#define DebugInfoStrDt(str, dt) ULUTil::LPrintDebugInformation(this, str, dt)

UCLASS(BlueprintType)
class L2WORLD_API ULUtil : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", DisplayName = "Print Debug Information (L2W)", Keywords = "print string text log", DevelopmentOnly))
	static void LPrintDebugInformation(FString InString, UObject* Source, float Duration = 5.f, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0));

};