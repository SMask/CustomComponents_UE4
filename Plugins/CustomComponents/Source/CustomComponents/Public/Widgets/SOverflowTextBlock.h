// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 超出长度省略号，仅单行
 */
class SOverflowTextBlock : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOverflowTextBlock)
		: _Text()
		, _TextStyle(&FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
		, _Font()
		, _ColorAndOpacity()
		, _Margin()
		{}
		SLATE_ATTRIBUTE(FText, Text)
		SLATE_STYLE_ARGUMENT(FTextBlockStyle, TextStyle)
		SLATE_ATTRIBUTE(FSlateFontInfo, Font)
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
		SLATE_ATTRIBUTE(FMargin, Margin)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

public:
	const FText& GetText() const;
private:
	FSlateFontInfo GetFont() const;
private:
	TAttribute<FText> Text;
	FTextBlockStyle TextStyle;
	TAttribute<FSlateFontInfo> Font;
	TAttribute<FSlateColor> ColorAndOpacity;
	TAttribute<FMargin> Margin;

	TSharedPtr<STextBlock> TextBlock;

public:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
};
