// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "NaMobSystemStyle.h"

class FNaMobSystemCommands : public TCommands<FNaMobSystemCommands>
{
public:

	FNaMobSystemCommands()
		: TCommands<FNaMobSystemCommands>(TEXT("NaMobSystem"), NSLOCTEXT("Contexts", "NaMobSystem", "NaMobSystem Plugin"), NAME_None, FNaMobSystemStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};