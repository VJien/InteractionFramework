// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_BlueprintLibrary.h"



TEnumAsByte<EWorldType::Type> UIF_BlueprintLibrary::GetWorldType(UObject* WorldContext)
{
	if (WorldContext && WorldContext->GetWorld())
	{
		return WorldContext->GetWorld()->WorldType;
	}
	return EWorldType::None;
}

 bool UIF_BlueprintLibrary::IsPIE_Mode(UObject* WorldContext)
{
	return GetWorldType(WorldContext) == EWorldType::PIE;
}

 bool UIF_BlueprintLibrary::IsEditorMode(UObject* WorldContext)
{
	return GetWorldType(WorldContext) == EWorldType::Editor;
}

 bool UIF_BlueprintLibrary::IsGameMode(UObject* WorldContext)
{
	return GetWorldType(WorldContext) == EWorldType::Game;
}
