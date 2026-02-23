// Copyright Epic Games, Inc. All Rights Reserved.

#include "NatriumMobSystem.h"
#include "NaMobSystemStyle.h"
#include "NaMobSystemCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName NaMobSystemTabName("NaMobSystem");

#define LOCTEXT_NAMESPACE "FNaMobSystemModule"

DEFINE_LOG_CATEGORY(LogNaMob)

void FNaMobSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	/*
	FNaMobSystemStyle::Initialize();
	FNaMobSystemStyle::ReloadTextures();

	FNaMobSystemCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FNaMobSystemCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FNaMobSystemModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNaMobSystemModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(NaMobSystemTabName, FOnSpawnTab::CreateRaw(this, &FNaMobSystemModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FNaMobSystemTabTitle", "NaMobSystem"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
		*/
}

void FNaMobSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	/*
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FNaMobSystemStyle::Shutdown();

	FNaMobSystemCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(NaMobSystemTabName);
	*/
}
/*
TSharedRef<SDockTab> FNaMobSystemModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FNaMobSystemModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("NaMobSystem.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FNaMobSystemModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(NaMobSystemTabName);
}

void FNaMobSystemModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FNaMobSystemCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FNaMobSystemCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}
*/
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNaMobSystemModule, NaMobSystem)