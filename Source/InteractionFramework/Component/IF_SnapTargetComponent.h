// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/IF_SceneComponent.h"
#include "Components/ActorComponent.h"
#include "IF_SnapTargetComponent.generated.h"


UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent))
class INTERACTIONFRAMEWORK_API UIF_SnapTargetComponent : public UIF_SceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_SnapTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
