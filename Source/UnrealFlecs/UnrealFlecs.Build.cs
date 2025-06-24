using UnrealBuildTool;

public class UnrealFlecs : ModuleRules
{
    public UnrealFlecs(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
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
        
        PublicDependencyModuleNames.AddRange(new[] {
            "FlecsLibrary"
        });
        
        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { ModuleDirectory + "/Public"});
        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { ModuleDirectory + "/Private"});
    }
}