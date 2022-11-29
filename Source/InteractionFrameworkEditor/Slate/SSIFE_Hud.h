// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class INTERACTIONFRAMEWORKEDITOR_API SSIFE_Hud : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSIFE_Hud)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply UpdateInputMapping();
};
