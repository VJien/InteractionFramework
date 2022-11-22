// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IF_GrabSourceComponent.h"
#include "Components/ActorComponent.h"
#include "IF_GrabTargetComponent.generated.h"


UENUM(BlueprintType)
enum class EIF_GrabStat: uint8
{
	None = 0,
	Main = 1,
	Secondary = 2,
	MarkForRelease = 3,
};


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

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UFUNCTION(BlueprintCallable)
	bool RefreshGrabStat();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasAnyOtherComponentBeGrab(UIF_GrabTargetComponent*& OtherComp);
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool BeGrab(UIF_GrabSourceComponent* SourceComponent, float Duration = 0);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeRelease();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyGrabComponentUpdate(UIF_GrabTargetComponent* OtherComp = nullptr, EIF_GrabStat GivenStat = EIF_GrabStat::Main);

	UPROPERTY(BlueprintReadWrite)
	UIF_GrabSourceComponent* GrabSourceComponent = nullptr;
	UPROPERTY(BlueprintReadWrite)
	bool bIsGrab = false;
	UPROPERTY(BlueprintReadWrite)
	UIF_GrabTargetComponent* OtherGrabTargetComponent = nullptr;
	UPROPERTY(BlueprintReadWrite)
	EIF_GrabStat GrabStat = EIF_GrabStat::None;

	
	//抓取方式
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_VRGrabRule GrabRule = EIF_VRGrabRule::Any;
	//优先级, 越小越优先!!
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	int32 GrabPriority = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_2HandGrabMainHandRightAxis MainHandRightAxis = EIF_2HandGrabMainHandRightAxis::Z;

	

	
};
