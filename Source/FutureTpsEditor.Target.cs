
// FutureTPS Game All Rights Reserved

using UnrealBuildTool;

public class FutureTpsEditorTarget : TargetRules
{
	public FutureTpsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange(new[] { "FutureTps" });
	}
}