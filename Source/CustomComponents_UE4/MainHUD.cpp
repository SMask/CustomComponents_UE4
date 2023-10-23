// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "Widgets/SCanvas.h"

void AMainHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SAssignNew(RootView, SCanvas)

		// SOverflowTextBlock
		+ SCanvas::Slot()
		.Position(FVector2D(200, 200))
		.Size(FVector2D(200, 50))
		[
			SNew(STextBlock)
				.Text(FText::FromString(TEXT("ABC012DEF345GHIJ6789")))
		];

	GEngine->GameViewport->AddViewportWidgetContent(RootView.ToSharedRef());
}
