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
	static FSlateColorBrush ColorBlack = FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

	ChildSlot
		[
			SNew(SCanvas)

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(100, 100))
				.Size(FVector2D(100, 25))
				[
					SNew(SOverflowTextBlock)
						.Text(FText::FromString(TEXT("123ABC012D4")))
				]

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(100, 130))
				.Size(FVector2D(100, 25))
				[
					SNew(SOverflowTextBlock)
						.Margin(FMargin(10, 0, 20, 0))
						.Text(FText::FromString(TEXT("123ABC012D4")))
				]

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(100, 160))
				.Size(FVector2D(100, 25))
				[
					SNew(SOverflowTextBlock)
						.Text(FText::FromString(TEXT("123ABC012DEF345GHIJ6789")))
				]

				// SOverflowTextBlock
				+ SCanvas::Slot()
				.Position(FVector2D(100, 190))
				.Size(FVector2D(100, 25))
				[
					SNew(SOverflowTextBlock)
						.Margin(FMargin(15, 0, 20, 0))
						.Text(FText::FromString(TEXT("123ABC012DEF345GHIJ6789")))
				]

				// SBorder
				+ SCanvas::Slot()
				.Position(FVector2D(100, 220))
				.Size(FVector2D(100, 25))
				[
					SNew(SBorder)
						.BorderImage(&ColorYellow)
				]

				// SButton
				+ SCanvas::Slot()
				.Position(FVector2D(100, 250))
				.Size(FVector2D(100, 25))
				[
					SNew(SButton)
						.OnClicked_Lambda([]()
							{
								UE_LOG(LogTemp, Log, TEXT("Button OnClicked"));

								FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Button")));

								return FReply::Handled();
							})
						.Text(FText::FromString(TEXT("Button")))
								.OnHovered_Lambda([]()
									{
										UE_LOG(LogTemp, Log, TEXT("Button OnHovered"));
									})
								.OnUnhovered_Lambda([]()
									{
										UE_LOG(LogTemp, Log, TEXT("Button OnUnhovered"));
									})
				]

				// SButton
				+ SCanvas::Slot()
				.Position(FVector2D(100, 280))
				.Size(FVector2D(100, 25))
				[
					SNew(SButton)
						.OnClicked_Lambda([this]()
							{
								UE_LOG(LogTemp, Log, TEXT("Button OnMovieSceneCapture"));

								OnMovieSceneCapture();

								return FReply::Handled();
							})
						.Text(FText::FromString(TEXT("MovieSceneCapture")))
				]
		];
}
void SRootWidget::OnMovieSceneCapture()
{
	FRequestPlaySessionParams Params;

	GEditor->RequestPlaySession(Params);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
