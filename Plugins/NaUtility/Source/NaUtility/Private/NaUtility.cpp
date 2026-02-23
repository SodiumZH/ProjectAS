// Copyright Epic Games, Inc. All Rights Reserved.

#include "NatriumUtility.h"

#define LOCTEXT_NAMESPACE "FNaUtilityModule"

//DEFINE_LOG_CATEGORY(LogTemp);

void FNaUtilityModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNaUtilityModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNaUtilityModule, NaUtility)

#define NAPACK_MOB_SYSTEM