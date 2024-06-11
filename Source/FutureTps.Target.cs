
// FutureTPS Game All Rights Reserved

using UnrealBuildTool;

public class FutureTpsTarget : TargetRules
{
	public FutureTpsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange(new[] { "FutureTps" });
	}
}