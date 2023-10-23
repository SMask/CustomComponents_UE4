// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomComponentsCommands.h"

#define LOCTEXT_NAMESPACE "FCustomComponentsModule"

void FCustomComponentsCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "CustomComponents", "Bring up CustomComponents window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
