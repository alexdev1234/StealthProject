// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StealthProject : ModuleRules
{
	public StealthProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
																	"CoreUObject",
																	"Engine", 
																	"InputCore",
																	"EnhancedInput",
																	"UMG",
																	"GameplayTags",
																	"GameplayTagsEditor",
																	"NavigationSystem",
																	"AIModule", 
																	"Niagara"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"Projects",
			"ApplicationCore"
		});
	}
}
