// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIFE_Hud.h"

#include "SlateOptMacros.h"
#include "GameFramework/InputSettings.h"
#include "InteractionFramework/Core/IFSetting.h"
#include "InteractionFramework/Lib/IF_BlueprintLibrary.h"
#include "InteractionFramework/Data/IFTypes.h"
#define LOCTEXT_NAMESPACE "AssetRenameManager"
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSIFE_Hud::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SButton)
		.OnClicked(this, &SSIFE_Hud::UpdateInputMapping)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("IF_Button1", "Update Input Mapping"))
		]
	];
	
}

FReply SSIFE_Hud::UpdateInputMapping()
{
	auto InputSetting = GetMutableDefault<UInputSettings>();
	auto IFSetting = UIFSetting::Get();
	if (!InputSetting || !IFSetting)
	{
		return FReply::Handled();
	}
	auto InputMapping = IFSetting->InputMapping;
	if (InputMapping.Num() == 0)
	{
		return FReply::Handled();
	}
	auto Device = UIF_BlueprintLibrary::GetHMDType();
	if (Device == EIF_VRHMDType::Unknown)
	{
		return FReply::Handled();
	}
	auto DeviceInputDataAsset = InputMapping.Find(Device);
	if (!DeviceInputDataAsset)
	{
		return FReply::Handled();
	}
	




	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


#undef LOCTEXT_NAMESPACE