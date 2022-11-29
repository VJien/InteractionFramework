
#pragma once

#include "Modules/ModuleManager.h"

class FInteractionFrameworkEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


private:
	void RegisterSettings();
	void UnregisterSettings();
	bool HandleSettingsSaved();
	void RegisterStyleAndCommands();
	void UnregisterStyleAndCommands();

	void PluginButtonClicked();
	void RegisterMenus();

	void AddToolBarExtension(FToolBarBuilder& Builder);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
