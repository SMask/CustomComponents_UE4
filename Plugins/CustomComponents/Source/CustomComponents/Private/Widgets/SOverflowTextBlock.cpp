// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SOverflowTextBlock.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SOverflowTextBlock::Construct(const FArguments& InArgs)
{
	Text = InArgs._Text;
	TextStyle = *InArgs._TextStyle;
	Font = InArgs._Font;
	ColorAndOpacity = InArgs._ColorAndOpacity;
	Margin = InArgs._Margin;

	TextDisplayContent = TextDisplayContentSource = Text.Get().ToString();

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

void SOverflowTextBlock::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// 判断需要显示的文本是否更改
	const FString TextContent = Text.Get().ToString();// 需要显示的文本
	if (!TextDisplayContentSource.Equals(TextContent))
	{
		TextDisplayContent = TextDisplayContentSource = Text.Get().ToString();
	}

	// 判断需要显示的文本是否超出View宽度
	const FString DisplayContent = TextDisplayContent;// 实际显示的文本
	const int32 AllottedWidth = AllottedGeometry.ToPaintGeometry().GetLocalSize().X - Margin.Get().Left - Margin.Get().Right;
	int32 TextWidth = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(DisplayContent, GetFont()).X;
	if (TextWidth > AllottedWidth)
	{
		const int32 Length = DisplayContent.Len();
		int32 Left = 0;
		int32 Right = Length - 1;
		while (Left <= Right)
		{
			const int32 Mid = (Left + Right) / 2;
			const FString TempString = DisplayContent.Left(Mid) + EllipsisStr;
			TextWidth = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(TempString, GetFont()).X;
			if (TextWidth < AllottedWidth) {
				Left = Mid + 1;
			}
			else
			{
				Right = Mid - 1;
			}
		}
		TextDisplayContent = DisplayContent.Left(Right) + EllipsisStr;
		TextBlock->SetText(FText::FromString(TextDisplayContent));
	}

	// 判断Hovered逻辑
	if (bStartHoveredTimer)
	{
		HoveredTimeTotal += InDeltaTime;
		if (HoveredTimeTotal >= HoveredTimeWait)
		{
			StopHoveredTimer();

			bWasHovered = true;

			OnHovered();
		}
	}
}

void SOverflowTextBlock::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);

	StartHoveredTimer();
}

void SOverflowTextBlock::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	// 因为添加了Hover等待时间，所以这里的bWasHovered需要自己来写逻辑，否则可以参考SButton的OnMouseEnter, OnMouseLeave方法
	//const bool bWasHovered = IsHovered();

	SCompoundWidget::OnMouseLeave(MouseEvent);

	StopHoveredTimer();

	if (bWasHovered)
	{
		OnUnhovered();
	}

	bWasHovered = false;
}

void SOverflowTextBlock::StartHoveredTimer()
{
	bStartHoveredTimer = true;
}

void SOverflowTextBlock::StopHoveredTimer()
{
	bStartHoveredTimer = false;
	HoveredTimeTotal = 0.0f;
}

void SOverflowTextBlock::OnHovered()
{
	UE_LOG(LogTemp, Log, TEXT("OnHovered"));
}

void SOverflowTextBlock::OnUnhovered()
{
	UE_LOG(LogTemp, Log, TEXT("OnUnhovered"));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
