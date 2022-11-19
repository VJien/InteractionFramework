// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_VR_TraceVisual.h"


// Sets default values
AIF_VR_TraceVisual::AIF_VR_TraceVisual()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AIF_VR_TraceVisual::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIF_VR_TraceVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIF_VR_TraceVisual::UpdateData(FVector Location, const TArray<FVector>& Path, bool bValidLocation)
{
	
}

void AIF_VR_TraceVisual::Show()
{
}

void AIF_VR_TraceVisual::Hidden()
{
}

