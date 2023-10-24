// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SOverflowTextBlock.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "Brushes/SlateColorBrush.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SOverflowTextBlock::Construct(const FArguments& InArgs)
{
	Text = InArgs._Text;
	TextStyle = *InArgs._TextStyle;
	Font = InArgs._Font;
	ColorAndOpacity = InArgs._ColorAndOpacity;
	Margin = InArgs._Margin;

	ChildSlot
		[
			SAssignNew(TextBlock, STextBlock)
				.Text(Text)
				.TextStyle(&TextStyle)
				.ColorAndOpacity(ColorAndOpacity)
				.Margin(Margin)
		];
}

const FText& SOverflowTextBlock::GetText() const
{
	return Text.Get();
}

FSlateFontInfo SOverflowTextBlock::GetFont() const
{
	return Font.IsSet() ? Font.Get() : TextStyle.Font;
}

int32 SOverflowTextBlock::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FString Content = TextBlock->GetText().ToString();
	const FVector2D ViewSize = AllottedGeometry.ToPaintGeometry().GetLocalSize();
	const FVector2D TextSize = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(Content, GetFont());

	const int32 AllottedWidth = ViewSize.X - Margin.Get().Left - Margin.Get().Right;
	int32 TextWidth = TextSize.X;
	if (TextWidth > AllottedWidth)
	{
		const int32 Length = Content.Len();
		int32 Left = 0;
		int32 Right = Length - 1;
		while (Left <= Right)
		{
			const int32 Mid = (Left + Right) / 2;
			const FString TempString = Content.Left(Mid) + TEXT("…");
			TextWidth = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(TempString, GetFont()).X;
			if (TextWidth < AllottedWidth) {
				Left = Mid + 1;
			}
			else
			{
				Right = Mid - 1;
			}
		}
		TextBlock->SetText(Content.Left(Right) + TEXT("…"));
	}
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
