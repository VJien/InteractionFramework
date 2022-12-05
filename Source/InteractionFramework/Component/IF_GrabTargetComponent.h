// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "IF_GrabTargetComponent.generated.h"


class UIF_GrabSourceComponent;

UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class INTERACTIONFRAMEWORK_API UIF_GrabTargetComponent : public USceneComponent
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool BeGrab(UIF_GrabSourceComponent* SourceComponent, EIF_GrabStat& OutStat);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeRelease();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyGrabComponentUpdate(UIF_GrabTargetComponent* OtherComp = nullptr, EIF_GrabStat GivenStat = EIF_GrabStat::Main);

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
	//哪只手抓取
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_VRHandType HandType = EIF_VRHandType::None;
	//抓取方式
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_VRGrabRule GrabRule = EIF_VRGrabRule::Any;
	//主手优先级, 越小越优先为主手!!
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	int32 GrabPriority = 0;
	//方向优先级, 双手抓取的时候, 越小越靠前!! 因为并非主手一定是靠后的那只手
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	int32 DirectionPriority = 0;
	/*
	 *主手右侧方向轴, 用于决定双手抓握时的Roll
	 * 如果是Lock, 那么Roll固定
	 * 如果是Custom, 那么需要调用SetCustomRightDirectionComponent()设置方向组件
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_2HandGrabMainHandRightAxis MainHandRightAxis = EIF_2HandGrabMainHandRightAxis::Z;
	//是否平滑抓取, 如果false, 那么即武器跟手保持位置一致(缺点是, 手的抖动会无差别反馈到武器上)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bSmoothGrab = true;
	//抓到手上以后吸附住, 只作用于单手操作
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bAttachAfterGrab= false;
	
	//平滑抓取的最大速度, 在快速移动的时候会接近于这个速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab", ClampMin=0.1))
	float GrabSpeedFar = 500.f;
	//平滑抓取在接近静止的时候会接近于这个速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab", ClampMin=0.1))
	float GrabSpeedClose = 200.f;
	//最大速度和最小速度过渡的速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	float GrabSpeedInterp = 100.0f;
	//近处的旋转速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	float GrabRotationSpeedClose = 100.0f;
	//位置变化如果小于此(同时也要满足旋转变化), 那么定义为静止状态, 平滑抓取速度更接近于最小速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	float GrabTransitionTolerance = 2.0f;
	//旋转(四元数quat的值)变化如果小于此(同时也要满足位置变化), 那么定义为静止状态, 平滑抓取速度更接近于最小速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Smooth Grab", meta=(EditCondition = "bSmoothGrab"))
	float GrabRotationTolerance = 0.1f;

	

	float CurrGrabRotationSpeed = 0.01f;
	
	float CurrGrabSpeed = 0.01f;
	bool bIsAttached = false;
	bool bIsGrabed = false;
	bool bIsTwoHandGrab = false;
	
};
