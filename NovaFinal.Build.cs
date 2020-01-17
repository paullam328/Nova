// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;


public class NovaFinal : ModuleRules
{
	public NovaFinal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AkAudio", "OnlineSubsystem", "OnlineSubsystemUtils", "UMG", "Slate", "SlateCore", "Http", "Json", "JsonUtilities", "EngineSettings"});

        PrivateDependencyModuleNames.AddRange(new string[] {  });


        if(Target.Platform == UnrealTargetPlatform.Win64)
        {

            DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

            //untimeDependencies.Add("{0}/Binaries/Win64/steam_appid.txt");
        }


        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        // DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //DynamicallyLoadedModuleNames.AddRange(new string[] { "OnlineSubsystemNull" , "OnlineSubsystemSteam" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        //PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }

}
