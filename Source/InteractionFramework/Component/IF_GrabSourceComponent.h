// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "IF_GrabSourceComponent.generated.h"

class UIF_GrabTargetComponent;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bMainHand = false;

	UPROPERTY(BlueprintReadWrite)
	UIF_GrabTargetComponent* MatchedTargetCompoennt = nullptr;
};
