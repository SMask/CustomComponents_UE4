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

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	void StartHoveredTimer();
	void StopHoveredTimer();

	void OnHovered();
	void OnUnhovered();

private:
	bool bStartHoveredTimer = false;// 是否启动Hover定时器
	const float HoveredTimeWait = 0.35f;// Hover执行等待时长
	float HoveredTimeTotal = 0.0f;// Hover定时器变量

	bool bWasHovered = false;// 是否已经Hovered，true则会执行OnUnhovered方法
};
