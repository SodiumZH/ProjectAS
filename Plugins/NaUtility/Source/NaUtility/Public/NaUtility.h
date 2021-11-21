// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Utility/Utility.h"
#include "Actors/Actors.h"
#include "Components/Components.h"


//DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, All);

class FNaUtilityModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
