// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_GrabSourceComponent.h"

#include "IF_GrabTargetComponent.h"


// Sets default values for this component's properties
UIF_GrabSourceComponent::UIF_GrabSourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIF_GrabSourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UIF_GrabSourceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UIF_GrabSourceComponent::Grab_Implementation(AActor* TargetActor, float Duration, FName Tag)
{
	if (!TargetActor)
	{
		return false;
	}
	auto MatchedComps = TargetActor->GetComponentsByTag(UIF_GrabTargetComponent::StaticClass(), Tag);
	if (MatchedComps.Num() == 0)
	{
		return false;
	}
	if (MatchedComps.Num() == 1)
	{
		MatchedTargetCompoennt = Cast<UIF_GrabTargetComponent>(MatchedComps[0]);
	}
	else
	{
		float MinDist = FLT_MAX;
		for (int32 i=0; i< MatchedComps.Num();++i)
		{
			if (auto C = Cast<UIF_GrabTargetComponent>(MatchedComps[i]))
			{
				const float Dist = (C->GetComponentLocation() - this->GetComponentLocation()).SizeSquared();
				if (Dist < MinDist)
				{
					MinDist = Dist;
					MatchedTargetCompoennt = C;
				}
			}
		}
	}
	if (!MatchedTargetCompoennt)
	{
		return false;
	}
	MatchedTargetCompoennt->BeGrab(this,Duration);






	
	
	return false;
}

