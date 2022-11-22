// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionFramework/Data/IFTypes.h"

#include "IF_GrabSourceComponent.generated.h"

class UIF_GrabTargetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIF_OnGrabEvent, AActor*, GrabActor, UIF_GrabTargetComponent*, TargetComponent);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIF_OnReleaseEvent);

UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent))
class INTERACTIONFRAMEWORK_API UIF_GrabSourceComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_GrabSourceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;



	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Grab(AActor* TargetActor, float Duration, FName Tag);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Release();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bMainHand = false;

	UPROPERTY(BlueprintReadWrite)
	UIF_GrabTargetComponent* MatchedTargetCompoennt = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	float GrabSpeed = 100.f;


	UPROPERTY(BlueprintReadWrite)
	AActor* GrabedActor = nullptr;


	UPROPERTY(BlueprintAssignable)
	FIF_OnGrabEvent OnGrab;
	UPROPERTY(BlueprintAssignable)
	FIF_OnGrabEvent OnRelease;
};
