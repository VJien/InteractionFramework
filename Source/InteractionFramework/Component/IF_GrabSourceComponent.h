// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/IF_SceneComponent.h"

#include "InteractionFramework/Data/IFTypes.h"

#include "IF_GrabSourceComponent.generated.h"


class UIF_GrabTargetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIF_OnGrabEvent, AActor*, GrabActor, UIF_GrabTargetComponent*, TargetComponent, EIF_GrabStat, Stat);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIF_OnReleaseEvent);

UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent))
class INTERACTIONFRAMEWORK_API UIF_GrabSourceComponent : public UIF_SceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_GrabSourceComponent();

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


	//如果Hand == None, 那么根据Tag匹配
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GrabComponent(UIF_GrabTargetComponent* TargetComponent,  EIF_GrabStat& OutStat);
	//如果Hand == None, 那么根据Tag匹配
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Grab(AActor* TargetActor,EIF_VRHandType Hand, FName Tag, EIF_GrabStat& OutStat, UIF_GrabTargetComponent*& TargetComponent);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Release();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure)
	AActor* GetGrabedActor();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure)
	bool HasGrabActor();
	
	UFUNCTION(BlueprintCallable)
	void NotifyGrabed(AActor* GrabActor, UIF_GrabTargetComponent* TargetComponent, EIF_GrabStat Stat);
	
	

protected:
	UPROPERTY(BlueprintReadWrite)
	AActor* GrabedActor = nullptr;
	UPROPERTY(BlueprintReadWrite)
	UIF_GrabTargetComponent* MatchedTargetCompoennt = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_VRHandType HandType = EIF_VRHandType::None;
	

	
public:
	UPROPERTY(BlueprintAssignable)
	FIF_OnGrabEvent OnPreGrab;
	UPROPERTY(BlueprintAssignable)
	FIF_OnGrabEvent OnGrab;
	UPROPERTY(BlueprintAssignable)
	FIF_OnGrabEvent OnRelease;
	
	
		
	

};
