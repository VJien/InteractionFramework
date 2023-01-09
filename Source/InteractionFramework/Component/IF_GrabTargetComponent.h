// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/IF_HandTypeComponent.h"
#include "Base/IF_SceneComponent.h"

#include "Components/ActorComponent.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "IF_GrabTargetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrabTargetEvent, UIF_GrabSourceComponent*, GrabSourceComponent);
class UIF_GrabSourceComponent;

UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class INTERACTIONFRAMEWORK_API UIF_GrabTargetComponent : public UIF_HandTypeComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_GrabTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void StartTwoHandGrab();
protected:
	UFUNCTION(BlueprintCallable)
	bool RefreshGrabStat(UIF_GrabSourceComponent* SourceComponent);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasAnyOtherComponentBeGrab(UIF_GrabTargetComponent*& OtherComp);
	UFUNCTION(BlueprintCallable)
	void SetCustomRightDirectionComponent(USceneComponent* Component);

	void CallGrabFinished();
	virtual void PreGrabAsMainHand(UIF_GrabSourceComponent* SourceComponent);
	virtual void PreGrabAsSecondHand(UIF_GrabSourceComponent* SourceComponent);
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasGrabAnything();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool BeGrab(UIF_GrabSourceComponent* SourceComponent, EIF_GrabStat& OutStat);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeRelease();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyGrabComponentUpdate(UIF_GrabTargetComponent* OtherComp = nullptr, EIF_GrabStat GivenStat = EIF_GrabStat::Main);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EIF_GrabStat GetGrabStat()
	{
		return GrabStat;
	};
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EIF_VRHandType GetHandType()
	{
		return HandType;
	}
	UFUNCTION(BlueprintCallable)
	void ResetAverageCalculation();




public:
	

	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnGrabTargetEvent OnPreGrab;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnGrabTargetEvent OnGrab;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnGrabTargetEvent OnRelease;
	
	UPROPERTY(BlueprintReadWrite)
	UIF_GrabSourceComponent* GrabSourceComponent = nullptr;
	UPROPERTY(BlueprintReadWrite)
	bool bIsGrabing = false;
	UPROPERTY(BlueprintReadWrite)
	UIF_GrabTargetComponent* OtherGrabTargetComponent = nullptr;
	UPROPERTY(BlueprintReadWrite)
	EIF_GrabStat GrabStat = EIF_GrabStat::None;
	UPROPERTY(BlueprintReadWrite)
	USceneComponent* RightDirectionComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bDebug = false;

	//抓取方式
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_VRGrabRule GrabRule = EIF_VRGrabRule::Any;
	//主手优先级, 越大越优先为主手!!
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|TwoHand")
	int32 GrabPriority = 0;
	//方向优先级, 双手抓取的时候, 越小越靠前!! 因为并非主手一定是靠后的那只手
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|TwoHand")
	int32 DirectionPriority = 0;

	//作为Roll计算的优先级, 取较大者, 如果一样就取主手
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|TwoHand")
	int32 RightAxisPriority = 0;
	/*
	 *主手右侧方向轴, 用于决定双手抓握时的Roll
	 * 如果是Lock, 那么Roll固定
	 * 如果是Custom, 那么需要调用SetCustomRightDirectionComponent()设置方向组件
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|TwoHand")
	EIF_2HandGrabMainHandRightAxis HandRightAxis = EIF_2HandGrabMainHandRightAxis::Z;

	//抓到手上以后吸附住, 只作用于单手操作
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bAttachAfterGrab= false;
	
	//是否平滑抓取, 如果false, 那么即武器跟手保持位置一致(缺点是, 手的抖动会无差别反馈到武器上)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab")
	bool bSmoothGrab = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	float SmoothGrabTransitionSpeed = 500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	float SmoothGrabRotationSpeed = 500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	float SmoothGrabTransitionTolerance = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	float SmoothGrabRotationTolerance = 5.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	bool bOnlySmoothGrabWhenFaraway = true;
	//根据距离设定自动的旋转速度, 尽量保证旋转和位移差不多时间完成
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	bool bUseAutoRotationSpeedFromTransition = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|AverageCalc")
	bool bAverageCalculation = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|AverageCalc", meta=(EditCondition = "bAverageCalculation"))
	int32 AverageNumber = 10;


	
	UPROPERTY(BlueprintReadWrite)
	float CurrRotationGrabSpeed = 0.01f;
	
	bool bIsAttached = false;
	bool bIsGrabed = false;
	bool bIsTwoHandGrab = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bTransitionIsNear = false;
	UPROPERTY(BlueprintReadWrite)
	bool bRotationIsNear = false;
	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> SavedLocation;
	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> SavedLocation_Other;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrAvgLocIndex = 0;
};
