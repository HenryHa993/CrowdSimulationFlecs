using UnrealBuildTool;

public class FlecsLibrary : ModuleRules
{
    public FlecsLibrary(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
        
        PublicIncludePaths.AddRange(new string[] { ModuleDirectory + "/Public" });
        PrivateIncludePaths.AddRange(new string[] { ModuleDirectory + "/Private" });
        
        if (Target.LinkType == TargetLinkType.Monolithic)
        {
            PublicDefinitions.Add("flecs_STATIC");
        }
        else
        {
            // When compiling flecs.c, make sure to define flecs_EXPORTS, for example by adding -Dflecs_EXPORTS to the compiler command.
            // https://www.flecs.dev/flecs/md_docs_2Quickstart.html
            PrivateDefinitions.Add("flecs_EXPORTS");
        }
    }
}