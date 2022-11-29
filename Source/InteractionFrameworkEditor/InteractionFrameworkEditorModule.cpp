#include "InteractionFrameworkEditorModule.h"

#include "InteractionFrameworkEditorStyle.h"
#include "InteractionFramworkEditorCommands.h"
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "InteractionFramework/Core/IFSetting.h"
#include "Slate/SSIFE_Hud.h"
#define LOCTEXT_NAMESPACE "InteractionFrameworkEditor"
void FInteractionFrameworkEditorModule::StartupModule()
{
	
	RegisterSettings();
	FCoreDelegates::OnPostEngineInit.AddLambda([this]()
		{
		RegisterStyleAndCommands();
		});
	
}

void FInteractionFrameworkEditorModule::ShutdownModule()
{
	UnregisterSettings();
	UnregisterStyleAndCommands();
}

void FInteractionFrameworkEditorModule::RegisterSettings()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsContainerPtr SettingsContainer = SettingModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("Interaction Framework Setting",
			LOCTEXT("IFCategoryName", "Interaction Framework"),
			LOCTEXT("IFCategoryDescription", "Game configuration for the Interaction Framework  game module"));

		ISettingsSectionPtr SettingsSection = SettingModule->RegisterSettings("Project", "Game", "Interaction Framework",
			LOCTEXT("SettingsName", "Interaction Framework"),
			LOCTEXT("SettingsDescription", "Configure my GameSetting"),
			GetMutableDefault<UIFSetting>());



		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FInteractionFrameworkEditorModule::HandleSettingsSaved);
		}
	}
}

void FInteractionFrameworkEditorModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Game", "Interaction Framework Setting");
	}
}

bool FInteractionFrameworkEditorModule::HandleSettingsSaved()
{
	UIFSetting* s = GetMutableDefault<UIFSetting>();
	if (s)
	{
		s->SaveConfig();
		return true;
	}
	return false;
}

void FInteractionFrameworkEditorModule::RegisterStyleAndCommands()
{
	FIFE_Style::Initialize();
	FIFE_Style::ReloadTextures();

	FIFE_Commands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FIFE_Commands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FInteractionFrameworkEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	//UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FInteractionFrameworkEditorModule::RegisterMenus));
	
	FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	//工具栏
	TSharedPtr<FExtender> ToolBarExtender = MakeShareable(new FExtender());
	ToolBarExtender->AddToolBarExtension("File",
										 EExtensionHook::Before, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FInteractionFrameworkEditorModule::AddToolBarExtension));

	LevelEditor.GetToolBarExtensibilityManager()->AddExtender(ToolBarExtender);
}

void FInteractionFrameworkEditorModule::UnregisterStyleAndCommands()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FIFE_Style::Shutdown();
	FIFE_Commands::Unregister();
}

void FInteractionFrameworkEditorModule::PluginButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("111"))
}

void FInteractionFrameworkEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FIFE_Commands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FIFE_Commands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FInteractionFrameworkEditorModule::AddToolBarExtension(FToolBarBuilder& Builder)
{
	auto ContextMenu = []()
	{

		return SNew(SSIFE_Hud);
	};
	
	Builder.AddSeparator("AdvancedFramework");
	Builder.AddComboButton(FUIAction(), FOnGetContent::CreateLambda(ContextMenu),
						   TAttribute<FText>(FText::FromString("Interaction Framework")),
						   TAttribute<FText>(FText::FromString("Interaction Framework Menu")),
						   FSlateIcon(FIFE_Style::GetStyleSetName(), "Interaction Framework.OpenMenu"));
	
}


#undef  LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInteractionFrameworkEditorModule, InteractionFrameworkEditor)
