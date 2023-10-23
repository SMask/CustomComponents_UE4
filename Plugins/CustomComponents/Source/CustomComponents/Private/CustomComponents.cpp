// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomComponents.h"
#include "CustomComponentsStyle.h"
#include "CustomComponentsCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/SRootWidget.h"

static const FName CustomComponentsTabName("CustomComponents");

#define LOCTEXT_NAMESPACE "FCustomComponentsModule"

void FCustomComponentsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCustomComponentsStyle::Initialize();
	FCustomComponentsStyle::ReloadTextures();

	FCustomComponentsCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCustomComponentsCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FCustomComponentsModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCustomComponentsModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CustomComponentsTabName, FOnSpawnTab::CreateRaw(this, &FCustomComponentsModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FCustomComponentsTabTitle", "CustomComponents"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FCustomComponentsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCustomComponentsStyle::Shutdown();

	FCustomComponentsCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CustomComponentsTabName);
}

TSharedRef<SDockTab> FCustomComponentsModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FCustomComponentsModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("CustomComponents.cpp"))
		);

	//return SNew(SDockTab)
	//	.TabRole(ETabRole::NomadTab)
	//	[
	//		// Put your tab content here!
	//		SNew(SBox)
	//		.HAlign(HAlign_Center)
	//		.VAlign(VAlign_Center)
	//		[
	//			SNew(STextBlock)
	//			.Text(WidgetText)
	//		]
	//	];

	// SRootWidget
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SRootWidget)
		];
}

void FCustomComponentsModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(CustomComponentsTabName);
}

void FCustomComponentsModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCustomComponentsCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomComponentsCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomComponentsModule, CustomComponents)
