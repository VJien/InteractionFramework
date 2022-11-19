// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IF_VR_TraceVisual.generated.h"

UCLASS()
class INTERACTIONFRAMEWORK_API AIF_VR_TraceVisual : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIF_VR_TraceVisual();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UFUNCTION()
	void UpdateData(FVector Location, const TArray<FVector>& Path, bool bValidLocation);
	UFUNCTION()
	void Show();
	UFUNCTION()
	void Hidden();
};
