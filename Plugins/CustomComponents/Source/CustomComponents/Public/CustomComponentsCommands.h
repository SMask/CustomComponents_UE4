// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CustomComponentsStyle.h"

class FCustomComponentsCommands : public TCommands<FCustomComponentsCommands>
{
public:

	FCustomComponentsCommands()
		: TCommands<FCustomComponentsCommands>(TEXT("CustomComponents"), NSLOCTEXT("Contexts", "CustomComponents", "CustomComponents Plugin"), NAME_None, FCustomComponentsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};