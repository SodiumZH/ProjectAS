// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class L2World : ModuleRules
{
	public L2World(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "NaUtility", "NaWidgets", "NaItemSystem", "NaMobSystem" });
	}
}
