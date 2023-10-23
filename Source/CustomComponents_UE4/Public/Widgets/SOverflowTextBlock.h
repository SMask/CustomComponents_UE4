// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 *
 */
class CUSTOMCOMPONENTS_UE4_API SOverflowTextBlock : public STextBlock
{
public:
	SLATE_BEGIN_ARGS(SOverflowTextBlock)
		{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
