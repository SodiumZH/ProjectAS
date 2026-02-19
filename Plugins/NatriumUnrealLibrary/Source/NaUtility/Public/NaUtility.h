// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Utility/Utility.h"
#include "Actors/NaUtilityActors.h"
#include "Components/NaUtilityComponents.h"
#include "NaPublicDependencies/NaPublicDependencies.h"

#define NAUTILITY_ENABLED

//DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, All);

class FNaUtilityModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
