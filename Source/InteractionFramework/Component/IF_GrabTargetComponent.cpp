﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_GrabTargetComponent.h"

#include "IF_GrabSourceComponent.h"
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
#if WITH_EDITOR
void UIF_GrabTargetComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetFName() ==  GET_MEMBER_NAME_CHECKED(ThisClass, HandType))
	{
		FName TagName = NAME_None;
		if (HandType == EIF_VRHandType::Left)
		{
			TagName = TEXT("Left");
			ComponentTags.Remove( TEXT("Right"));
		}
		else if (HandType == EIF_VRHandType::Right)
		{
			TagName = TEXT("Right");
			ComponentTags.Remove( TEXT("Left"));
		}
		else
		{
			ComponentTags.Remove( TEXT("Right"));
			ComponentTags.Remove( TEXT("Left"));
		}
		
		
		if (TagName != NAME_None && !ComponentTags.Contains(TagName))
		{
			ComponentTags.Emplace(TagName);
		}
	}
}

#endif

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
			int32 DirScaleByPriority = 1;
			if (OtherGrabTargetComponent->DirectionPriority > DirectionPriority)
			{
				DirScaleByPriority = -1;
			}
			//两手的方向
			FVector V0 = (OtherGrabTargetComponent->GetComponentLocation() - GetComponentLocation())* DirScaleByPriority;
			V0.Normalize();
			//武器自身朝向
			FVector V1 = GetOwner()->GetActorForwardVector();
			//武器中的2个抓取点的朝向
        	FVector V2 = (OtherLoc - SourceTrans.GetLocation()) * DirScaleByPriority;
        	V2.Normalize();
			//两个抓取点与武器自身朝向的夹角, 后面需要补偿回来
			float Angle = UKismetMathLibrary::DegAcos(V1 | V2);
        	FVector Dir = UKismetMathLibrary::RotateAngleAxis(V0, Angle, V1 ^ V2);
			Dir.Normalize();
			if (bDebug)
			{
				UE_LOG(LogTemp, Log, TEXT("Angle =%f, Dir = %s"), Angle, *Dir.ToString())
			}
			FRotator TargetRot;
			if (MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::Lock)
			{
				TargetRot = UKismetMathLibrary::MakeRotFromXZ(Dir, FVector(0,0,1));
			}
			else if(MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::Custom && RightDirectionComponent)
			{
				TargetRot = UKismetMathLibrary::MakeRotFromXY(Dir, RightDirectionComponent->GetForwardVector());
			}
			else
			{
				FVector Y;
				if (MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::X)
				{
					Y = UKismetMathLibrary::GetForwardVector(SourceTrans.Rotator());
				}
				else if (MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::Y)
				{
					Y = UKismetMathLibrary::GetRightVector(SourceTrans.Rotator());
				}
				else if (MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::Z)
				{
					Y = UKismetMathLibrary::GetUpVector(SourceTrans.Rotator());
				}
				else if (MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::NegX)
				{
					Y = UKismetMathLibrary::GetForwardVector(SourceTrans.Rotator()) * -1;
				}
				else if (MainHandRightAxis == EIF_2HandGrabMainHandRightAxis::NegY)
				{
					Y = UKismetMathLibrary::GetRightVector(SourceTrans.Rotator()) * -1;
				}
				else
				{
					Y = UKismetMathLibrary::GetUpVector(SourceTrans.Rotator()) * -1;
				}
				TargetRot = UKismetMathLibrary::MakeRotFromXY(Dir, Y);
			}
			
			if (bDebug)
			{
				UE_LOG(LogTemp, Log, TEXT("TargetRot = %s"), *TargetRot.ToString())
			}
			FTransform OwnerTransform = GetOwner()->GetActorTransform();
			FTransform TargetActorTransform =  OwnerTransform.GetRelativeTransform(GetComponentTransform());
			FTransform TargetTransform = TargetActorTransform * GrabSourceComponent->GetComponentTransform();
			TargetTransform.SetRotation(TargetRot.Quaternion());
			if (bSmoothGrab)
			{
				if (UKismetMathLibrary::NearlyEqual_TransformTransform(OwnerTransform,TargetTransform,GrabTransitionTolerance,GrabRotationTolerance))
				{
					CurrGrabSpeed = UKismetMathLibrary::FInterpTo(CurrGrabSpeed, GrabSpeedMin, DeltaTime, GrabSpeedInterp);
				}
				else
				{
					CurrGrabSpeed = UKismetMathLibrary::FInterpTo(CurrGrabSpeed, GrabSpeedMax, DeltaTime, GrabSpeedInterp);
				}
				if (bDebug)
				{
					UE_LOG(LogTemp, Log, TEXT("OwnerTransform = %s, TargetTransform = %s"), *OwnerTransform.ToString(), *TargetTransform.ToString())
				}
				const FTransform CurrTransform = UKismetMathLibrary::TInterpTo(OwnerTransform, TargetTransform,DeltaTime,CurrGrabSpeed);
				if (bDebug)
				{
					UE_LOG(LogTemp, Log, TEXT("CurrTransform = %s"), *CurrTransform.ToString())
				}
				GetOwner()->SetActorLocationAndRotation(CurrTransform.GetLocation(),CurrTransform.GetRotation());
			}
			else
			{
				if (bDebug)
				{
					UE_LOG(LogTemp, Log, TEXT("OwnerTransform = %s, TargetTransform = %s"), *OwnerTransform.ToString(), *TargetTransform.ToString())
				}
				GetOwner()->SetActorLocationAndRotation(TargetTransform.GetLocation(),TargetRot);
			}
		
		}
		else // 单手抓取
		{
			FTransform OwnerTransform = GetOwner()->GetActorTransform();
			FTransform TargetActorTransform =  OwnerTransform.GetRelativeTransform(GetComponentTransform());
			FTransform TargetTransform = TargetActorTransform * GrabSourceComponent->GetComponentTransform();
			if (bSmoothGrab)
			{
				if (UKismetMathLibrary::NearlyEqual_TransformTransform(OwnerTransform,TargetTransform,GrabTransitionTolerance,GrabRotationTolerance))
				{
					CurrGrabSpeed = UKismetMathLibrary::FInterpTo(CurrGrabSpeed, GrabSpeedMin, DeltaTime, GrabSpeedInterp);
				}
				else
				{
					CurrGrabSpeed = UKismetMathLibrary::FInterpTo(CurrGrabSpeed, GrabSpeedMax, DeltaTime, GrabSpeedInterp);
				}
			
				const FTransform CurrTransform = UKismetMathLibrary::TInterpTo(OwnerTransform, TargetTransform,DeltaTime,CurrGrabSpeed);
				GetOwner()->SetActorLocationAndRotation(CurrTransform.GetLocation(),CurrTransform.GetRotation());
			}
			else
			{
				GetOwner()->SetActorLocationAndRotation(TargetTransform.GetLocation(),TargetTransform.GetRotation());
			}
		
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

void UIF_GrabTargetComponent::SetCustomRightDirectionComponent(USceneComponent* Component)
{
	RightDirectionComponent = Component;
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

bool UIF_GrabTargetComponent::BeGrab_Implementation(UIF_GrabSourceComponent* SourceComponent, EIF_GrabStat& OutStat)
{
	if (!SourceComponent)
	{
		return false;
	}
	if (RefreshGrabStat())
	{
		bIsGrab = true;
		GrabSourceComponent = SourceComponent;
		OutStat = GrabStat;
		return true;
	}
	
	return false;
}

PRAGMA_ENABLE_OPTIMIZATION