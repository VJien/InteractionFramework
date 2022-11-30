// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_HandAniminstance.h"

void UIF_HandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	InterFingerData(DeltaSeconds);
}

void UIF_HandAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
}

void UIF_HandAnimInstance::InterFingerData(float Delta)
{
	if (!bInterp)
	{
		return;
	}
	Interp(CurrPose.Thumbstick, TargetPose.Thumbstick, Delta, InterpSpeed_GrabBlend, InterpSpeed_Rotation);
	Interp(CurrPose.Index, TargetPose.Index, Delta, InterpSpeed_GrabBlend, InterpSpeed_Rotation);
	Interp(CurrPose.Middle, TargetPose.Middle, Delta, InterpSpeed_GrabBlend, InterpSpeed_Rotation);
	Interp(CurrPose.Ring, TargetPose.Ring, Delta, InterpSpeed_GrabBlend, InterpSpeed_Rotation);
	Interp(CurrPose.Pinky, TargetPose.Pinky, Delta, InterpSpeed_GrabBlend, InterpSpeed_Rotation);
}

void UIF_HandAnimInstance::Interp(FIF_VRHandFingerData& Curr, FIF_VRHandFingerData Target,
	float Delta, float BlendSpeed, float RotSpeed)
{
	if (Curr.IsNealyEqual(Target))
	{
		return;
	}
	Curr.GrabBlend = FMath::FInterpConstantTo(Curr.GrabBlend, Target.GrabBlend, Delta, BlendSpeed);
	Curr.Rotation_Joint1 = FMath::RInterpConstantTo(Curr.Rotation_Joint1, Target.Rotation_Joint1, Delta, RotSpeed);
	Curr.Rotation_Joint2 = FMath::RInterpConstantTo(Curr.Rotation_Joint2, Target.Rotation_Joint2, Delta, RotSpeed);
	Curr.Rotation_Joint3 = FMath::RInterpConstantTo(Curr.Rotation_Joint3, Target.Rotation_Joint3, Delta, RotSpeed);
}

void UIF_HandAnimInstance::UpdateFingerData_Implementation(EIF_HandFingerType Finger, const FIF_VRHandFingerData& Data)
{
	if (!bInterp)
	{
		switch (Finger)
		{
		case EIF_HandFingerType::Thumbstick:
			{
				CurrPose.Thumbstick = Data;
				TargetPose.Thumbstick = Data;
				return;
			}
		case EIF_HandFingerType::Index:
			{
				CurrPose.Index = Data;
				TargetPose.Index = Data;
				return;
			}
		case EIF_HandFingerType::Middle:
			{
				CurrPose.Middle = Data;
				TargetPose.Middle = Data;
				return;
			}
		case  EIF_HandFingerType::Ring:
			{
				CurrPose.Ring = Data;
				TargetPose.Ring = Data;
				return;
			}
		case  EIF_HandFingerType::Pinky:
			{
				CurrPose.Pinky = Data;
				TargetPose.Pinky = Data;
				return;
			}
		default:
			break;
		}
	}
	else
	{
		switch (Finger)
		{
		case EIF_HandFingerType::Thumbstick:
			{
				TargetPose.Thumbstick = Data;
				return;
			}
		case EIF_HandFingerType::Index:
			{
				TargetPose.Index = Data;
				return;
			}
		case EIF_HandFingerType::Middle:
			{
				TargetPose.Middle = Data;
				return;
			}
		case  EIF_HandFingerType::Ring:
			{
				TargetPose.Ring = Data;
				return;
			}
		case  EIF_HandFingerType::Pinky:
			{
				TargetPose.Pinky = Data;
				return;
			}
		default:
			break;
		}
	}
	
}

void UIF_HandAnimInstance::UpdateAllFingerData_Implementation(FIF_VRHandPoseData Data)
{
	if (!bInterp)
	{
		CurrPose = Data;
		TargetPose = Data;
	}
	else
	{
		TargetPose = Data;
	}
	
}

void UIF_HandAnimInstance::UpdateFingersData_Implementation(
	const TMap<EIF_HandFingerType, FIF_VRHandFingerData>& FingersData)
{
	if (FingersData.Num() == 0)
	{
		return;
	}
	for (auto P: FingersData)
	{
		Execute_UpdateFingerData(this,P.Key, P.Value);
	}
}

