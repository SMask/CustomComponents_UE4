// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SRootWidget.h"
#include "Widgets/SCanvas.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRootWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew( SCanvas)

			// SOverflowTextBlock
			+ SCanvas::Slot()
			.Position(FVector2D(200, 200))
			.Size(FVector2D(200, 50))
			[
				SNew(STextBlock)
					.Text(FText::FromString(TEXT("ABC012DEF345GHIJ6789")))
			]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
