// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProgrammingFinal : ModuleRules
{
	public ProgrammingFinal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //Added dependencies: Paper2D, PaperZD, Niagara, NiagaraCore
        PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"Paper2D",
			"PaperZD",
            "Niagara",
			"NiagaraCore"
        });

        //Added dependencies: Niagara, NiagaraCore
        PrivateDependencyModuleNames.AddRange(new string[] {
		    "Niagara",
			"NiagaraCore"
        });

		PublicIncludePaths.AddRange(new string[] {
			"ProgrammingFinal",
			"ProgrammingFinal/Variant_Horror",
			"ProgrammingFinal/Variant_Horror/UI",
			"ProgrammingFinal/Variant_Shooter",
			"ProgrammingFinal/Variant_Shooter/AI",
			"ProgrammingFinal/Variant_Shooter/UI",
			"ProgrammingFinal/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
