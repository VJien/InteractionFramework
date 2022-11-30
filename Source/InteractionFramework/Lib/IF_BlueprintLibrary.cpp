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

FIF_VRHandFingerData UIF_BlueprintLibrary::LerpFingerData(FIF_VRHandFingerData A, FIF_VRHandFingerData B, float Alpha)
{
	return A.Lerp(B,Alpha);
}

FIF_VRHandFingerData UIF_BlueprintLibrary::GetFingerData(FIF_VRHandPoseData PoseData, EIF_HandFingerType Finger)
{
	switch (Finger)
	{
	case EIF_HandFingerType::Thumbstick:
		{
			return PoseData.Thumbstick;
		}
	case EIF_HandFingerType::Index:
		{
			return PoseData.Index;
		}
	case EIF_HandFingerType::Middle:
		{
			return PoseData.Middle;
		}
	case EIF_HandFingerType::Ring:
		{
			return PoseData.Ring;
		}
	case EIF_HandFingerType::Pinky:
		{
			return PoseData.Pinky;
		}
		default:break;
	}
	return FIF_VRHandFingerData();
}

FString UIF_BlueprintLibrary::InputTypeToString(EIF_VRInputType InputType)
{
	return IFEnumToString<EIF_VRInputType>("EIF_VRInputType",InputType);
}

EIF_VRInputType UIF_BlueprintLibrary::StringToInputType(FString String)
{
	return IFStringToEnum<EIF_VRInputType>("EIF_VRInputType",String);
}
