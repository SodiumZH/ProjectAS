#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "L2World.h"
#include "LUtil.generated.h"

#define DebugInfo(text) ULUTil::LPrintDebugInformation(this, text)
#define DebugInfoDt(text, dt) ULUTil::LPrintDebugInformation(this, text, dt)

UCLASS(BlueprintType)
class L2WORLD_API ULUtil : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", DisplayName = "Print Debug Information (L2W)", Keywords = "print string text log", DevelopmentOnly))
	static void LPrintDebugInformation(FString InString, UObject* Source, float Duration = 5.f, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0));

};