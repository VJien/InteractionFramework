// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "UObject/Interface.h"
#include "IF_HandPoseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIF_HandAnimPoseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONFRAMEWORK_API IIF_HandAnimPoseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateFingerData(EIF_HandFingerType Finger, const FIF_VRHandFingerData& Data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateAllFingerData(FIF_VRHandPoseData Data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateFingersData(const TMap<EIF_HandFingerType, FIF_VRHandFingerData>& FingersData);
};
