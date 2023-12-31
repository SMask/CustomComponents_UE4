// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMCOMPONENTS_UE4_API AMainHUD : public AHUD
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() override;

	TSharedPtr<SWidget> RootView;
	
};
