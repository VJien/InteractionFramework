// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_SceneComponent.h"

bool UIF_SceneComponent::IsWorking()
{
	return bIsWorking;
}

void UIF_SceneComponent::EnableWorking(bool bEnable)
{
	bIsWorking = bEnable;
}
