// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BoxDeleterActorEditor : ModuleRules
{
	public BoxDeleterActorEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add(
				"UnrealEd");
		}
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{ 
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
