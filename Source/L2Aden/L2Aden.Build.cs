// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class L2Aden : ModuleRules
{
	public L2Aden(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
