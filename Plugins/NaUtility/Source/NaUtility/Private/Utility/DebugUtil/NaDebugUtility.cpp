#pragma once

#include "Utility/DebugUtil/NaDebugUtility.h"
#include "Logging/LogMacros.h"
#include "Kismet/KismetSystemLibrary.h"

void UNaDebugUtility::DebugInfo(UObject* Source, FString Info, float Time, bool bShowSource, bool bPrintToScreen, bool bPrintToLog, FLinearColor Color) {

	if (bShowSource)
		Info = Info + TEXT(" Source: ") + UKismetSystemLibrary::GetDisplayName(Source);
	
	UKismetSystemLibrary::PrintString(Source, Info, bPrintToScreen, bPrintToLog, Color, Time);
}

FString UNaDebugUtility::DisplayName(UObject* InObj) {
	return UKismetSystemLibrary::GetDisplayName(InObj);
}

