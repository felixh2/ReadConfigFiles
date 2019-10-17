// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ReadFromConfigFileTarget : TargetRules
{
	public ReadFromConfigFileTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "ReadFromConfigFile" } );
	}
}
