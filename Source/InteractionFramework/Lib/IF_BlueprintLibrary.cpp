// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_BlueprintLibrary.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "IXRTrackingSystem.h"
#include "openvr.h"
#include "OpenVRExpansionFunctionLibrary.h"


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

EIF_VRHMDType UIF_BlueprintLibrary::GetHMDType()
{

	const auto Version = UHeadMountedDisplayFunctionLibrary::GetVersionString();
	const auto DeviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName().ToString();
	if (DeviceName.Contains("PSVR"))
	{
		return EIF_VRHMDType::PSVR;
	}
	else if (Version.Find("index", ESearchCase::IgnoreCase) != INDEX_NONE)
	{
		return EIF_VRHMDType::ValveIndex;
	}
	else if (Version.Find("vive_cosmos", ESearchCase::IgnoreCase) != INDEX_NONE)
	{
		return EIF_VRHMDType::ViveCosmos;
	}
	else if (Version.Contains(TEXT("vive")))
	{
		if (Version.Find("focus3", ESearchCase::IgnoreCase) != INDEX_NONE)
		{
			return EIF_VRHMDType::ViveFocus3;
		}
		else if (Version.Find("focus", ESearchCase::IgnoreCase) != INDEX_NONE)
		{
			return EIF_VRHMDType::ViveFocus;
		}
		else if (Version.Find("Pico  Neo3", ESearchCase::IgnoreCase) != INDEX_NONE)
		{
			return EIF_VRHMDType::PicoNeo3;
		}
		else if (Version.Find("Pico  Neo4", ESearchCase::IgnoreCase) != INDEX_NONE)
		{
			return EIF_VRHMDType::PicoNeo4;
		}
		else
		{
			return EIF_VRHMDType::Vive;
		}
		
	}
	else if ((Version.Find("oculus quest", ESearchCase::IgnoreCase) != INDEX_NONE) ||
					(Version.Find("miramar", ESearchCase::IgnoreCase) != INDEX_NONE))
	{
		return EIF_VRHMDType::OculusQuestHMD;
	}
	else if (Version.Find("oculus", ESearchCase::IgnoreCase) != INDEX_NONE)
	{
		return EIF_VRHMDType::OculusHMD;
	}
	else if (
		Version.Find("Mixed Reality", ESearchCase::IgnoreCase) != INDEX_NONE ||
		Version.Find("Acer", ESearchCase::IgnoreCase) != INDEX_NONE ||
		Version.Find("Lenovo", ESearchCase::IgnoreCase) != INDEX_NONE
		)
	{
		return EIF_VRHMDType::WindowsMR;
	}
	else 
	{
		auto Type = UOpenVRExpansionFunctionLibrary::GetOpenVRHMDType();
		return static_cast<EIF_VRHMDType>(static_cast<int32>(Type));
	}

	return EIF_VRHMDType::Unknown;
	
}
