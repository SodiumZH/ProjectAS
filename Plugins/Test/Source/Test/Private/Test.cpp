// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test.h"
#include "TestStyle.h"
#include "TestCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SNaHorizontalBar.h"
#include "ToolMenus.h"

static const FName TestTabName("Test");

#define LOCTEXT_NAMESPACE "FTestModule"

void FTestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTestStyle::Initialize();
	FTestStyle::ReloadTextures();

	FTestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTestCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FTestModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTestModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TestTabName, FOnSpawnTab::CreateRaw(this, &FTestModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FTestTabTitle", "Test"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FTestStyle::Shutdown();

	FTestCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TestTabName);
}

TSharedRef<SDockTab> FTestModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FTestModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Test.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				//SNew(STextBlock)
				//.Text(WidgetText)
				SNew(SNaHorizontalBar)
				.ImageBar(LoadObject<UTexture2D>(nullptr, TEXT("/NaWidgets/UI/Gauge_DF_Large_HP_Center.Gauge_DF_Large_HP_Center")))
				.ImageLeftEnd(LoadObject<UTexture2D>(nullptr, TEXT("/NaWidgets/UI/Gauge_DF_Large_HP_Left.Gauge_DF_Large_HP_Left")))
				.ImageRightEnd(LoadObject<UTexture2D>(nullptr, TEXT("/NaWidgets/UI/Gauge_DF_Large_HP_Right.Gauge_DF_Large_HP_Right")))
			]
		];
}

void FTestModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(TestTabName);
}

void FTestModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FTestCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FTestCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestModule, Test)