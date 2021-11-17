#pragma once

#include "NaDebugUtility.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogNaPack);

void LNaDebugUtility::DebugInfo(UObject* Source, FString Info, float Time, bool bShowSource, bool bPrintToScreen, bool bPrintToLog, FLinearColor Color) {

	if (bShowSource)
		Info = Info + TEXT(" Source: ") + UKismetSystemLibrary::GetDisplayName(Source);
	
	if (NAPACK_ENABLE_DEBUG)
		UKismetSystemLibrary::PrintString(Source, Info, bPrintToScreen, bPrintToLog, Color, Time);
}

FString LNaDebugUtility::DisplayName(UObject* InObj) {
	return UKismetSystemLibrary::GetDisplayName(InObj);
}

