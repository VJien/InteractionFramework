// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "IF_HandPoseInterface.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "IF_HandAniminstance.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIONFRAMEWORK_API UIF_HandAnimInstance : public UAnimInstance, public IIF_HandAnimPoseInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
	
	UFUNCTION()
	void InterFingerData(float Delta);

	
	void Interp(FIF_VRHandFingerData& Current,  FIF_VRHandFingerData Target, float Delta, float BlendSpeed, float RotSpeed);
public:
	virtual void UpdateFingerData_Implementation(EIF_HandFingerType Finger, const FIF_VRHandFingerData& Data)override;
	virtual void UpdateAllFingerData_Implementation(FIF_VRHandPoseData Data)override;
	virtual void UpdateFingersData_Implementation(const TMap<EIF_HandFingerType, FIF_VRHandFingerData>& FingersData) override;	

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bInterp = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	float BlendTime = 0.2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	float InterpSpeed_GrabBlend = 5.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	float InterpSpeed_Rotation = 90.f;


	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Runtime)
	FIF_VRHandPoseData CurrPose;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Runtime)
	FIF_VRHandPoseData TargetPose;

public:
		
	
	
};
