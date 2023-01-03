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
	void PreGrabAsMainHand(UIF_GrabSourceComponent* SourceComponent);
	void PreGrabAsSecondHand(UIF_GrabSourceComponent* SourceComponent);
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
	
protected:
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	int32 GrabPriority = 0;
	//方向优先级, 双手抓取的时候, 越小越靠前!! 因为并非主手一定是靠后的那只手
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	int32 DirectionPriority = 0;

	//作为Roll计算的优先级, 取较大者, 如果一样就取主手
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	int32 RightAxisPriority = 0;
	/*
	 *主手右侧方向轴, 用于决定双手抓握时的Roll
	 * 如果是Lock, 那么Roll固定
	 * 如果是Custom, 那么需要调用SetCustomRightDirectionComponent()设置方向组件
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_2HandGrabMainHandRightAxis HandRightAxis = EIF_2HandGrabMainHandRightAxis::Z;
	//是否平滑抓取, 如果false, 那么即武器跟手保持位置一致(缺点是, 手的抖动会无差别反馈到武器上)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bSmoothGrab = true;
	//抓到手上以后吸附住, 只作用于单手操作
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bAttachAfterGrab= false;
	
	//平滑抓取的最大速度, 在快速移动的时候会接近于这个速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Transition", meta=(EditCondition = "bSmoothGrab", ClampMin=0.1))
	float GrabSpeedFar = 500.f;
	//平滑抓取在接近静止的时候会接近于这个速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Transition", meta=(EditCondition = "bSmoothGrab", ClampMin=0.1))
	float GrabSpeedClose = 200.f;
	//最大速度和最小速度过渡的速度, 双重差值, 保证平滑过渡; 值小于等于0就不做差值
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Transition", meta=(EditCondition = "bSmoothGrab"))
	float GrabSpeedInterpToFar = 100.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Transition", meta=(EditCondition = "bSmoothGrab"))
	float GrabSpeedInterpToClose = 100.0f;
	//位置变化如果小于此(同时也要满足旋转变化), 那么定义为静止状态, 平滑抓取速度更接近于最小速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Transition", meta=(EditCondition = "bSmoothGrab"))
	float GrabTransitionTolerance = 2.0f;
	
	//近处的旋转速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Rotation", meta=(EditCondition = "bSmoothGrab"))
	float GrabRotationSpeedFar = 1000.0f;
	//近处的旋转速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Rotation", meta=(EditCondition = "bSmoothGrab"))
	float GrabRotationSpeedClose = 100.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Rotation", meta=(EditCondition = "bSmoothGrab"))
	float GrabRotationSpeedInterpToFar = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Rotation", meta=(EditCondition = "bSmoothGrab"))
	float GrabRotationSpeedInterpToClose = 1.0f;
	//旋转(四元数quat的值)变化如果小于此(同时也要满足位置变化), 那么定义为静止状态, 平滑抓取速度更接近于最小速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab|Rotation", meta=(EditCondition = "bSmoothGrab"))
	float GrabRotationTolerance = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bAverageCalculation = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition = "bAverageCalculation"))
	int32 AverageNumber = 10;


	
	//开始抓取的时候的抓取旋转速度
	UPROPERTY(BlueprintReadWrite)
	float StartGrabRotationSpeed = 0.01f;
	UPROPERTY(BlueprintReadWrite)
	float CurrGrabSpeed = 0.01f;
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
