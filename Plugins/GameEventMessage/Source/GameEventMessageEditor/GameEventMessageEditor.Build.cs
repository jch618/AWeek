// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameEventMessageEditor: ModuleRules
{
	public GameEventMessageEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"BlueprintGraph",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "UnrealEd",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore", "GameEventMessage",
				"Kismet",
				"KismetCompiler",
				"GraphEditor",
				"EditorSubsystem",
				// ... add private dependencies that you statically link with here ...	
			}
			);
	}
}
