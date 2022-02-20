// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test.h"
#include "TestStyle.h"
#include "TestCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "Widgets/BoxSlots/SNaSlotList.h"

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
	/*
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FTestModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Test.cpp"))
		);
	*/
	/*
	FNaBoxSlotParams Params = FNaBoxSlotParams();
	Params.SubscriptText = FText::FromString(TEXT("123"));
	Params.SuperscriptText = FText::FromString(TEXT("abc"));
	Params.ImageBase = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/NaItemSystem/UI/Resource/Image/ItemSlot_Disabled.ItemSlot_Disabled'"));
	Params.ImageFrame = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/NaItemSystem/UI/Resource/Image/BoxSlotFrame.BoxSlotFrame'"));
	Params.ImagePointed = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/NaItemSystem/UI/Resource/Image/itembox_pointed.itembox_pointed'"));
	Params.ImageSelected = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/NaItemSystem/UI/Resource/Image/itemslot_selected.itemslot_selected'"));
	*/

	FNaSlotListDisplayInfo ListInfo = FNaSlotListDisplayInfo();
	ListInfo.SubscriptTextArray[0] = FText::FromString(TEXT("123"));
	ListInfo.SuperscriptTextArray[0] = FText::FromString(TEXT("abc"));
	ListInfo.ImageBaseArray[0] = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/NaItemSystem/UI/Resource/Image/ItemSlot_Disabled.ItemSlot_Disabled'"));
	ListInfo.ImageFrameArray[0] = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/NaItemSystem/UI/Resource/Image/BoxSlotFrame.BoxSlotFrame'"));
	ListInfo.ImagePointed = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/NaItemSystem/UI/Resource/Image/itembox_pointed.itembox_pointed'"));
	ListInfo.ImageSelected = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/NaItemSystem/UI/Resource/Image/itemslot_selected.itemslot_selected'"));
	ListInfo.Length = 32;
	ListInfo.BoxSize = FVector2D(64, 64);
	ListInfo.FixArrays();


	TSharedPtr<SNaSlotList> SlotList;


	TSharedRef<SDockTab> ReturnWidget = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			/*/
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
			*/
			SAssignNew(SlotList, SNaSlotList)
			.DisplayInfo(&ListInfo)
		];
	//BoxSlot->SetSelected(true);
	return ReturnWidget;
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