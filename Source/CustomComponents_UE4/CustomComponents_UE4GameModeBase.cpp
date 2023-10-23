// Copyright Epic Games, Inc. All Rights Reserved.


#include "CustomComponents_UE4GameModeBase.h"
#include "MainHUD.h"

ACustomComponents_UE4GameModeBase::ACustomComponents_UE4GameModeBase()
{
	HUDClass = AMainHUD::StaticClass();
}
