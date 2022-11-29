// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionFramworkEditorCommands.h"

#define LOCTEXT_NAMESPACE "FIFE_Commands"

void FIFE_Commands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Interaction Framework", "Interaction Framework", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
