// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionFrameworkEditorStyle.h"

class FIFE_Commands : public TCommands<FIFE_Commands>
{
public:

	FIFE_Commands()
		: TCommands<FIFE_Commands>(TEXT("Interaction Framework"), NSLOCTEXT("Contexts", "FIFE_Commands", "Interaction Framework"), NAME_None, FIFE_Style::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
