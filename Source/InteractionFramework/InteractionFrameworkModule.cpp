// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "InteractionFrameworkModule.h"

#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Core/IFSetting.h"
#define LOCTEXT_NAMESPACE "InteractionFrameworkSetting"
void FInteractionFrameworkModule::StartupModule()
{
	RegisterSettings();
}

void FInteractionFrameworkModule::ShutdownModule()
{
	UnregisterSettings();
}

void FInteractionFrameworkModule::RegisterSettings()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsContainerPtr SettingsContainer = SettingModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("Interaction Framework Setting",
			LOCTEXT("IFCategoryName", "Interaction Framework Setting"),
			LOCTEXT("IFCategoryDescription", "Game configuration for the Interaction Framework  game module"));

		ISettingsSectionPtr SettingsSection = SettingModule->RegisterSettings("Project", "Game", "Interaction Framework Setting",
			LOCTEXT("SettingsName", "Interaction Framework Setting"),
			LOCTEXT("SettingsDescription", "Configure my GameSetting"),
			GetMutableDefault<UIFSetting>());



		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FInteractionFrameworkModule::HandleSettingsSaved);
		}
	}
}

void FInteractionFrameworkModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Game", "Interaction Framework Setting");
	}
}

bool FInteractionFrameworkModule::HandleSettingsSaved()
{
	UIFSetting* s = GetMutableDefault<UIFSetting>();
	if (s)
	{
		s->SaveConfig();
		return true;
	}
	return false;
}


#undef  LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInteractionFrameworkModule, InteractionFramework)
