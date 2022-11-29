// Fill out your copyright notice in the Description page of Project Settings.


#include "IFTypes.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(IF_Log)

bool FIF_VRHandFingerData::IsNealyEqual(FIF_VRHandFingerData Other, float BlendTolerance, float RotationTolerance)
{
	return FMath::IsNearlyEqual(GrabBlend, Other.GrabBlend,BlendTolerance) &&
		Rotation_Joint1.Equals(Other.Rotation_Joint1,RotationTolerance) &&
			Rotation_Joint2.Equals(Other.Rotation_Joint2,RotationTolerance) &&
				Rotation_Joint3.Equals(Other.Rotation_Joint3,RotationTolerance);
}
