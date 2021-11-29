// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestCommands.h"

#define LOCTEXT_NAMESPACE "FTestModule"

void FTestCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Test", "Execute Test action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
