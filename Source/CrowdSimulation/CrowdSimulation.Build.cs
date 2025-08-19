// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrowdSimulation : ModuleRules
{
	public CrowdSimulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "StateTreeModule", "GameplayStateTreeModule" });
		
		PublicDependencyModuleNames.AddRange(new[] {
			"FlecsLibrary",
			"UnrealFlecs",
			"TurboSequence_Lf",
			"ImGui"
		});
	}
}
