// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_ActorComponent.h"


// Sets default values for this component's properties
UIF_ActorComponent::UIF_ActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIF_ActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UIF_ActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UIF_ActorComponent::IsWorking()
{
	return bIsWorking;
}

void UIF_ActorComponent::EnableWorking(bool bEnable)
{
	bIsWorking = bEnable;
}

