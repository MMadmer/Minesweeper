// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperTool.h"
#include "MinesweeperToolStyle.h"
#include "MinesweeperToolCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "UI/SMinesweeperWidget.h"

static const FName MinesweeperToolTabName("MinesweeperTool");

#define LOCTEXT_NAMESPACE "FMinesweeperToolModule"

void FMinesweeperToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FMinesweeperToolStyle::Initialize();
	FMinesweeperToolStyle::ReloadTextures();

	FMinesweeperToolCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeperToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperToolModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperToolTabName,
	                                                  FOnSpawnTab::CreateRaw(
		                                                  this, &FMinesweeperToolModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FMinesweeperToolTabTitle", "MinesweeperTool"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeperToolStyle::Shutdown();

	FMinesweeperToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperToolTabName);
}

TSharedRef<SDockTab> FMinesweeperToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SMinesweeperWidget)
		];
}

void FMinesweeperToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperToolTabName);
}

void FMinesweeperToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");

		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
		Section.AddMenuEntryWithCommandList(FMinesweeperToolCommands::Get().OpenPluginWindow, PluginCommands);
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");

		FToolMenuEntry& Entry = Section.AddEntry(
			FToolMenuEntry::InitToolBarButton(FMinesweeperToolCommands::Get().OpenPluginWindow));
		Entry.SetCommandList(PluginCommands);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMinesweeperToolModule, MinesweeperTool)
