#pragma once

#include "LUtil.h"
#include "Kismet/KismetSystemLibrary.h"

void ULUtil::LPrintDebugInformation(FString InString, UObject* Source, float Duration, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor) {
	FString Name = UKismetSystemLibrary::GetDisplayName(Source);
	FString Output = FString(TEXT("Debug Info: \"")) + InString + FString(TEXT("\" Source: \"")) + Name + FString(TEXT("\""));
	
	bool ToScreen = ENABLE_DEBUG_OUTPUT_TO_SCREEN ? bPrintToScreen : false;
	bool ToLog = ENABLE_DEBUG_OUTPUT_TO_LOG ? bPrintToLog : false;
	UKismetSystemLibrary::PrintString(Source, Output, ToScreen, ToLog, TextColor, Duration);
}


