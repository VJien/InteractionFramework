// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "InteractionFrameworkModule.h"

#define LOCTEXT_NAMESPACE "InteractionFrameworkSetting"
void FInteractionFrameworkModule::StartupModule()
{
}

void FInteractionFrameworkModule::ShutdownModule()
{
}



#undef  LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInteractionFrameworkModule, InteractionFramework)
