// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMCOMPONENTS_UE4_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	virtual void SetupInputComponent() override;

	void OnEndPressed();
};
