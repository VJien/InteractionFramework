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
#if WITH_EDITOR
void UIF_GrabSourceComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	
}

#endif

// Called every frame
void UIF_GrabSourceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UIF_GrabSourceComponent::NotifyGrabed(AActor* GrabActor, UIF_GrabTargetComponent* TargetComponent,
	EIF_GrabStat Stat)
{
	OnGrab.Broadcast(GrabActor, TargetComponent, Stat);
}



bool UIF_GrabSourceComponent::HasGrabActor_Implementation()
{
	return GrabedActor != nullptr;
}

AActor* UIF_GrabSourceComponent::GetGrabedActor_Implementation()
{
	return GrabedActor;
}

void UIF_GrabSourceComponent::Release_Implementation()
{
	if (MatchedTargetCompoennt)
	{
		MatchedTargetCompoennt->BeRelease();
	}
	OnRelease.Broadcast(GrabedActor, MatchedTargetCompoennt, MatchedTargetCompoennt->GetGrabStat());
	MatchedTargetCompoennt = nullptr;
	GrabedActor = nullptr;
	
}

bool UIF_GrabSourceComponent::Grab_Implementation(AActor* TargetActor, EIF_VRHandType Hand, FName Tag, EIF_GrabStat& OutStat)
{
	if (!TargetActor)
	{
		return false;
	}
	FName CompTag = Tag;
	if (Hand == EIF_VRHandType::Left)
	{
		CompTag = TEXT("Left");
	}
	else if(Hand == EIF_VRHandType::Right)
	{
		CompTag = TEXT("Right");
	}
	auto MatchedComps = TargetActor->GetComponentsByTag(UIF_GrabTargetComponent::StaticClass(), CompTag);
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
	if (MatchedTargetCompoennt->BeGrab(this,OutStat))
	{
		GrabedActor = TargetActor;
		OnPreGrab.Broadcast(TargetActor, MatchedTargetCompoennt,MatchedTargetCompoennt->GetGrabStat());
	}
	
	
	return true;
}

