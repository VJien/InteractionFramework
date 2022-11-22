﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_GrabTargetComponent.h"

#include "Kismet/KismetMathLibrary.h"

PRAGMA_DISABLE_OPTIMIZATION
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
	if (bIsGrab && GrabStat == EIF_GrabStat::Main && GrabSourceComponent)
	{
		if (OtherGrabTargetComponent)
		{
			FVector OtherLoc = OtherGrabTargetComponent->GrabSourceComponent->GetComponentLocation();
			FTransform SourceTrans =  GrabSourceComponent->GetComponentTransform();
			FVector Dir = OtherLoc - SourceTrans.GetLocation();
			Dir.Normalize();
			FVector Y;
			if (MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::X)
			{
				Y = UKismetMathLibrary::GetForwardVector(SourceTrans.Rotator());
			}
			else if (MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::Y)
			{
				Y = UKismetMathLibrary::GetRightVector(SourceTrans.Rotator());
			}
			else
			{
				Y = UKismetMathLibrary::GetUpVector(SourceTrans.Rotator());
			}
			
			FRotator TargetRot = UKismetMathLibrary::MakeRotFromXY(Dir, Y);
			
			FTransform OwnerTransform = GetOwner()->GetActorTransform();
			FTransform TargetActorTransform =  OwnerTransform.GetRelativeTransform(GetComponentTransform());
			FTransform TargetTransform = TargetActorTransform * GrabSourceComponent->GetComponentTransform();
			TargetTransform.SetRotation(TargetRot.Quaternion());
			
			const FTransform CurrTransform = UKismetMathLibrary::TInterpTo(OwnerTransform, TargetTransform,DeltaTime,GrabSourceComponent->GrabSpeed);
			GetOwner()->SetActorLocationAndRotation(CurrTransform.GetLocation(),CurrTransform.GetRotation());
		}
		else
		{
			FTransform OwnerTransform = GetOwner()->GetActorTransform();
			FTransform TargetActorTransform =  OwnerTransform.GetRelativeTransform(GetComponentTransform());
			FTransform TargetTransform = TargetActorTransform * GrabSourceComponent->GetComponentTransform();
			const FTransform CurrTransform = UKismetMathLibrary::TInterpTo(OwnerTransform, TargetTransform,DeltaTime,GrabSourceComponent->GrabSpeed);
			GetOwner()->SetActorLocationAndRotation(CurrTransform.GetLocation(),CurrTransform.GetRotation());
		}
	}
	// ...
}

bool UIF_GrabTargetComponent::RefreshGrabStat()
{
	if (HasAnyOtherComponentBeGrab(OtherGrabTargetComponent))
	{
		switch (GrabRule)
		{
		case EIF_VRGrabRule::Any:
			{
				if (OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::Any)
				{
					//当前的比其他的优先级高
					if (GrabPriority < OtherGrabTargetComponent->GrabPriority)
					{
						GrabStat = EIF_GrabStat::Main;
						OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Secondary);
						return true;
					}
					else
					{
						GrabStat = EIF_GrabStat::Secondary;
						OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Main);
						return true;
					}
					
				}
				else if(OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::AlwaysMainHand)
				{
					GrabStat = EIF_GrabStat::Secondary;
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Main);
					return true;
				}
				else
				{
					GrabStat = EIF_GrabStat::Main;
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Secondary);
					return true;
				}
				
			}
		case EIF_VRGrabRule::AlwaysMainHand:
			{
				if (OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::Any)
				{
					GrabStat = EIF_GrabStat::Main;
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Secondary);
					return true;
				}
				else if(OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::AlwaysMainHand)
				{
					//当前的比其他的优先级高
					if (GrabPriority < OtherGrabTargetComponent->GrabPriority )
					{
						GrabStat = EIF_GrabStat::Main;
						OtherGrabTargetComponent->GrabSourceComponent->Release();
						OtherGrabTargetComponent = nullptr;
						return true;
					}
					else
					{
						GrabStat = EIF_GrabStat::MarkForRelease;
						OtherGrabTargetComponent->NotifyGrabComponentUpdate();
						if (GrabSourceComponent)
						{
							GrabSourceComponent->Release();
						}
						return false;
					}
				}
				else // Secondary
				{
					GrabStat = EIF_GrabStat::Main;
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Secondary);
					return true;
				}
			}
		case EIF_VRGrabRule::AlwaysSecondaryHand:
			{
				if (OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::Any)
				{
					GrabStat = EIF_GrabStat::Secondary;
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Main);
					return true;
				}
				else if(OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::AlwaysMainHand)
				{
					GrabStat = EIF_GrabStat::Secondary;
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Main);
					return true;
				}
				else // Secondary, 2个副手, 不能抓取
					{
					break;
					}
			}
			default:
				break;
		}
		
	
	}
	else
	{
		if (GrabRule != EIF_VRGrabRule::AlwaysSecondaryHand)
		{
			GrabStat = EIF_GrabStat::Main;
			return true;
		}
	}
	return false;
}

bool UIF_GrabTargetComponent::HasAnyOtherComponentBeGrab(UIF_GrabTargetComponent*& OtherComp)
{
	TArray<UIF_GrabTargetComponent*> TargetComps;
	GetOwner()->GetComponents<UIF_GrabTargetComponent>(TargetComps);
	TargetComps.Remove(this);
	if (TargetComps.Num() == 0)
	{
		OtherComp = nullptr;
		return false;
	}
	for (auto C : TargetComps)
	{
		if (UIF_GrabTargetComponent* Target = Cast<UIF_GrabTargetComponent>(C))
		{
			if (Target->GrabStat != EIF_GrabStat::None && Target->GrabStat != EIF_GrabStat::MarkForRelease)
			{
				OtherComp = Target;
				return true;
			}
		}
	}
	OtherComp = nullptr;
	return false;
}

void UIF_GrabTargetComponent::NotifyGrabComponentUpdate_Implementation(UIF_GrabTargetComponent* OtherComp, EIF_GrabStat GivenStat)
{
	if (OtherComp)
	{
		OtherGrabTargetComponent = OtherComp;
		GrabStat = GivenStat;
	}
	else
	{
		RefreshGrabStat();
	}
}

void UIF_GrabTargetComponent::BeRelease_Implementation()
{
	bIsGrab = false;
	GrabStat = EIF_GrabStat::None;
	if (OtherGrabTargetComponent)
	{
		OtherGrabTargetComponent->NotifyGrabComponentUpdate();
	}
	OtherGrabTargetComponent = nullptr;
}

bool UIF_GrabTargetComponent::BeGrab_Implementation(UIF_GrabSourceComponent* SourceComponent, float Duration)
{
	if (!SourceComponent)
	{
		return false;
	}
	if (RefreshGrabStat())
	{
		bIsGrab = true;
		GrabSourceComponent = SourceComponent;
		return true;
	}
	
	return false;
}

PRAGMA_ENABLE_OPTIMIZATION