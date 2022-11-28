// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_InputManager.h"

void UIF_InputManager::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeChainProperty(PropertyChangedEvent);
	if (!InputSettings.IsValid())
	{
		InputSettings = GetMutableDefault<UInputSettings>();
	}
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, InputMapping))
	{
		
	}
	
}

void UIF_InputManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (!InputSettings.IsValid())
	{
		InputSettings = GetMutableDefault<UInputSettings>();
	}
}
