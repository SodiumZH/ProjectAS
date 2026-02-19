#pragma once

#include "Utility/DebugUtil/NaDebugUtility.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogNaUtil);
DEFINE_LOG_CATEGORY(LogNaWidgets);
DEFINE_LOG_CATEGORY(LogNaItem);
DEFINE_LOG_CATEGORY(LogNaMob);
DEFINE_LOG_CATEGORY(LogNaPackTemp);

void UNaDebugUtility::DebugInfo(UObject* Source, FString Info, float Time, bool bShowSource, bool bPrintToScreen, bool bPrintToLog, FLinearColor Color) {

	if (bShowSource)
		Info = Info + TEXT(" Source: ") + UKismetSystemLibrary::GetDisplayName(Source);
	
	UKismetSystemLibrary::PrintString(Source, Info, bPrintToScreen, bPrintToLog, Color, Time);
}

FString UNaDebugUtility::DisplayName(UObject* InObj) {
	return UKismetSystemLibrary::GetDisplayName(InObj);
}

