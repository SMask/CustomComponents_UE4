// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SRootWidget.h"
#include "Widgets/SCanvas.h"
#include "Widgets/SOverflowTextBlock.h"
#include "Brushes/SlateColorBrush.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRootWidget::Construct(const FArguments& InArgs)
{
	static FSlateColorBrush ColorYellow = FSlateColorBrush(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));

	ChildSlot
		[
			SNew(SCanvas)

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(200, 200))
				.Size(FVector2D(100, 25))
				[
					SNew(SOverflowTextBlock)
						.Text(FText::FromString(TEXT("123ABC012D4")))
				]

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(200, 230))
				.Size(FVector2D(100, 25))
				[
					SNew(SOverflowTextBlock)
						.Margin(FMargin(10, 0, 20, 0))
						.Text(FText::FromString(TEXT("123ABC012D4")))
				]

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(200, 260))
				.Size(FVector2D(100, 25))
				[
					SNew(SOverflowTextBlock)
						.Text(FText::FromString(TEXT("123ABC012DEF345GHIJ6789")))
				]

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(200, 290))
				.Size(FVector2D(100, 25))
				[
					SNew(SOverflowTextBlock)
						.Margin(FMargin(15, 0, 20, 0))
						.Text(FText::FromString(TEXT("123ABC012DEF345GHIJ6789")))
				]

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(200, 320))
				.Size(FVector2D(100, 25))
				[
					SNew(SBorder)
						.BorderImage(&ColorYellow)
				]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
