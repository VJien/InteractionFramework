// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_GrabTargetComponent.h"


// Sets default values for this component's properties
UIF_GrabTargetComponent::UIF_GrabTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIF_GrabTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UIF_GrabTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsGrab && GrabStat == EIF_GrabStat::Main)
	{
		
	}
	// ...
}

void UIF_GrabTargetComponent::RefreshGrabStat()
{
	UIF_GrabTargetComponent* OtherComp = nullptr;
	if (HasAnyOtherComponentBeGrab(OtherComp))
	{
		GrabStat = OtherComp->GrabPriority < GrabPriority? EIF_GrabStat::Secondary : EIF_GrabStat::Main;
	}
	else
	{
		GrabStat = EIF_GrabStat::Main;
	}
}

bool UIF_GrabTargetComponent::HasAnyOtherComponentBeGrab(UIF_GrabTargetComponent*& OtherComp)
{
	auto TargetComps = GetOwner()->GetComponentsByClass(UIF_GrabTargetComponent::StaticClass());
	if (TargetComps.Num() == 0)
	{
		return false;
	}
	for (auto C : TargetComps)
	{
		if (UIF_GrabTargetComponent* Target = Cast<UIF_GrabTargetComponent>(C))
		{
			if (Target->GrabStat != EIF_GrabStat::None)
			{
				OtherComp = Target;
				return true;
			}
		}
	}
	return false;
}

void UIF_GrabTargetComponent::NotifyGrabComponentUpdate_Implementation()
{
	RefreshGrabStat();
}

void UIF_GrabTargetComponent::BeRelease_Implementation()
{
	
}

bool UIF_GrabTargetComponent::BeGrab_Implementation(UIF_GrabSourceComponent* SourceComponent, float Duration)
{
	if (!SourceComponent)
	{
		return false;
	}
	RefreshGrabStat();
	bIsGrab = true;
	
	return false;
}

