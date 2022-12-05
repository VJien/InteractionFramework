// Fill out your copyright notice in the Description page of Project Settings.


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
	if ( (bIsGrabing ||  bIsTwoHandGrab)  && GrabStat == EIF_GrabStat::Main && GrabSourceComponent )
	{
		//双手抓取
		if (OtherGrabTargetComponent)
		{
			bIsTwoHandGrab = true;
			FVector OtherLoc = OtherGrabTargetComponent->GrabSourceComponent->GetComponentLocation();
			FTransform SourceTrans =  GrabSourceComponent->GetComponentTransform();
			int32 DirScaleByPriority = 1;
			if (OtherGrabTargetComponent->DirectionPriority > DirectionPriority)
			{
				DirScaleByPriority = -1;
			}
			//两手的方向
			FVector V0 = (OtherLoc - SourceTrans.GetLocation()) * DirScaleByPriority;
			V0.Normalize();
			//武器自身朝向
			FVector V1 = GetOwner()->GetActorForwardVector();
			//武器中的2个抓取点的朝向
        	FVector V2 = (OtherGrabTargetComponent->GetComponentLocation() - GetComponentLocation())* DirScaleByPriority;
        	V2.Normalize();
			FRotator DeltaRot;
			
			DeltaRot = UKismetMathLibrary::FindLookAtRotation( GetOwner()->GetTransform().InverseTransformPosition(GetComponentLocation()) ,
				GetOwner()->GetTransform().InverseTransformPosition(OtherGrabTargetComponent->GetComponentLocation()));
			DeltaRot = DeltaRot.GetInverse();
			if (DirScaleByPriority < 0)
			{
				DeltaRot = DeltaRot.GetInverse();
			}
			
			FRotator HandRot =  UKismetMathLibrary::FindLookAtRotation( SourceTrans.GetTranslation(), OtherLoc);
			if (DirScaleByPriority < 0)
			{
				HandRot = HandRot.GetInverse();
			}
			FRotator TargetRot = UKismetMathLibrary::ComposeRotators(HandRot, DeltaRot);
		
			FVector Dir = TargetRot.Vector();
			
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
				FRotator CurrRot;
				if (UKismetMathLibrary::NearlyEqual_TransformTransform(OwnerTransform,TargetTransform,GrabTransitionTolerance,GrabRotationTolerance))
				{
					CurrGrabSpeed = UKismetMathLibrary::FInterpTo(CurrGrabSpeed, GrabSpeedClose, DeltaTime, GrabSpeedInterp);
					CurrRot = UKismetMathLibrary::RInterpTo_Constant(OwnerTransform.GetRotation().Rotator(), TargetTransform.GetRotation().Rotator(),DeltaTime, GrabRotationSpeedClose);
				}
				else
				{
					CurrGrabSpeed = UKismetMathLibrary::FInterpTo(CurrGrabSpeed, GrabSpeedFar, DeltaTime, GrabSpeedInterp);
					CurrRot = UKismetMathLibrary::RInterpTo_Constant(OwnerTransform.GetRotation().Rotator(), TargetTransform.GetRotation().Rotator(),DeltaTime, CurrGrabRotationSpeed);
				}
				if (bDebug)
				{
					UE_LOG(LogTemp, Log, TEXT("OwnerTransform = %s, TargetTransform = %s"), *OwnerTransform.ToString(), *TargetTransform.ToString())
				}
				FVector CurrLoc = UKismetMathLibrary::VInterpTo_Constant(OwnerTransform.GetLocation(), TargetTransform.GetLocation(),DeltaTime,CurrGrabSpeed);
				 
				
				if (bDebug)
				{
					UE_LOG(LogTemp, Log, TEXT("CurrTransform = %s"), *FTransform(CurrRot, CurrLoc).ToString())
				}
				
				GetOwner()->SetActorLocationAndRotation(CurrLoc,CurrRot);
				CallGrabFinished();
			}
			else
			{
				if (bDebug)
				{
					UE_LOG(LogTemp, Log, TEXT("OwnerTransform = %s, TargetTransform = %s"), *OwnerTransform.ToString(), *TargetTransform.ToString())
				}
				CallGrabFinished();
				GetOwner()->SetActorLocationAndRotation(TargetTransform.GetLocation(),TargetRot);
			}
		
		}
		else // 单手抓取
		{
			bIsTwoHandGrab = false;
			if (bIsAttached)
			{
				return;
			}
			FTransform OwnerTransform = GetOwner()->GetActorTransform();
			FTransform TargetActorTransform =  OwnerTransform.GetRelativeTransform(GetComponentTransform());
			FTransform TargetTransform = TargetActorTransform * GrabSourceComponent->GetComponentTransform();
			if (bSmoothGrab)
			{
				FRotator CurrRot;
				if (UKismetMathLibrary::NearlyEqual_TransformTransform(OwnerTransform,TargetTransform,GrabTransitionTolerance,GrabRotationTolerance))
				{
					if (bAttachAfterGrab)
					{
						bIsAttached = true;
						GetOwner()->SetActorLocationAndRotation(TargetTransform.GetLocation(),TargetTransform.GetRotation());
						GetOwner()->AttachToComponent(GrabSourceComponent, FAttachmentTransformRules::KeepWorldTransform);
						return;
					}
					else
					{
						CurrRot = UKismetMathLibrary::RInterpTo_Constant(OwnerTransform.GetRotation().Rotator(), TargetTransform.GetRotation().Rotator(),DeltaTime,GrabRotationSpeedClose);
						CurrGrabSpeed = UKismetMathLibrary::FInterpTo(CurrGrabSpeed, GrabSpeedClose, DeltaTime, GrabSpeedInterp);
					}
					CallGrabFinished();
				}
				else
				{
					CurrGrabSpeed = UKismetMathLibrary::FInterpTo(CurrGrabSpeed, GrabSpeedFar, DeltaTime, GrabSpeedInterp);
					CurrRot = UKismetMathLibrary::RInterpTo_Constant(OwnerTransform.GetRotation().Rotator(), TargetTransform.GetRotation().Rotator(),DeltaTime,CurrGrabRotationSpeed);
				}
				FVector CurrLoc = UKismetMathLibrary::VInterpTo_Constant(OwnerTransform.GetLocation(), TargetTransform.GetLocation(),DeltaTime,CurrGrabSpeed);
				GetOwner()->SetActorLocationAndRotation(CurrLoc,CurrRot);
			}
			else
			{
				GetOwner()->SetActorLocationAndRotation(TargetTransform.GetLocation(),TargetTransform.GetRotation());
				if (bAttachAfterGrab)
				{
					bIsAttached = true;
					GetOwner()->AttachToComponent(GrabSourceComponent, FAttachmentTransformRules::KeepWorldTransform);
				}
				CallGrabFinished();
				return;
				
			}
		
		}
	}
	// ...
}

void UIF_GrabTargetComponent::StartTwoHandGrab()
{
	bIsTwoHandGrab = true;
}

bool UIF_GrabTargetComponent::RefreshGrabStat(UIF_GrabSourceComponent* SourceComponent)
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
						PreGrabAsMainHand(SourceComponent);
						OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Secondary);
						return true;
					}
					else
					{
						PreGrabAsSecondHand(SourceComponent);
						OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Main);
						return true;
					}
					
				}
				else if(OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::AlwaysMainHand)
				{
					PreGrabAsSecondHand(SourceComponent);
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Main);
					return true;
				}
				else
				{
					PreGrabAsMainHand(SourceComponent);
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Secondary);
					return true;
				}
				
			}
		case EIF_VRGrabRule::AlwaysMainHand:
			{
				if (OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::Any)
				{
					PreGrabAsMainHand(SourceComponent);
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Secondary);
					return true;
				}
				else if(OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::AlwaysMainHand)
				{
					//当前的比其他的优先级高
					if (GrabPriority < OtherGrabTargetComponent->GrabPriority )
					{
						PreGrabAsMainHand(SourceComponent);
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
					PreGrabAsMainHand(SourceComponent);
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Secondary);
					return true;
				}
			}
		case EIF_VRGrabRule::AlwaysSecondaryHand:
			{
				if (OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::Any)
				{
					PreGrabAsSecondHand(SourceComponent);
					OtherGrabTargetComponent->NotifyGrabComponentUpdate(this, EIF_GrabStat::Main);
					return true;
				}
				else if(OtherGrabTargetComponent->GrabRule == EIF_VRGrabRule::AlwaysMainHand)
				{
					PreGrabAsSecondHand(SourceComponent);
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
			PreGrabAsMainHand(SourceComponent);
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

void UIF_GrabTargetComponent::CallGrabFinished()
{
	if (!bIsGrabed)
	{
		bIsGrabed = true;
		if (bAttachAfterGrab)
		{
			bIsGrabing = false;
		}
		if (GrabSourceComponent)
		{
			GrabSourceComponent->NotifyGrabed(GetOwner(), this, GrabStat);	
		}
	}
}

void UIF_GrabTargetComponent::PreGrabAsMainHand(UIF_GrabSourceComponent* SourceComponent)
{
	GrabStat = EIF_GrabStat::Main;
	bIsGrabing = true;
	bIsTwoHandGrab = false;
	bIsAttached = false;

	if (SourceComponent)
	{
		GrabSourceComponent = SourceComponent;
		//设置抓取旋转速度
		float Dist =  (GetComponentLocation() - SourceComponent->GetComponentLocation()).Size();
		if (Dist <= GrabTransitionTolerance)
		{
			CurrGrabRotationSpeed = GrabRotationSpeedClose;
		}
		else
		{
			FTransform OwnerTransform = GetOwner()->GetActorTransform();
			FTransform TargetActorTransform =  OwnerTransform.GetRelativeTransform(GetComponentTransform());
			FTransform TargetTransform = TargetActorTransform * GrabSourceComponent->GetComponentTransform();
			const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator (OwnerTransform.GetRotation().Rotator(), TargetTransform.GetRotation().Rotator());
			CurrGrabRotationSpeed = FMath::Max3(FMath::Abs(DeltaRot.Roll), FMath::Abs(DeltaRot.Pitch), FMath::Abs(DeltaRot.Yaw));
			CurrGrabRotationSpeed *= GrabSpeedFar / Dist;
		}
		
		CurrGrabSpeed = GrabSpeedClose;
	}
	
}

void UIF_GrabTargetComponent::PreGrabAsSecondHand(UIF_GrabSourceComponent* SourceComponent)
{
	GrabSourceComponent = SourceComponent;
	GrabStat = EIF_GrabStat::Secondary;
}

void UIF_GrabTargetComponent::NotifyGrabComponentUpdate_Implementation(UIF_GrabTargetComponent* OtherComp, EIF_GrabStat GivenStat)
{
	if (OtherComp)
	{
		OtherGrabTargetComponent = OtherComp;
		GrabStat = GivenStat;
		if (GrabStat == EIF_GrabStat::Main)
		{
			StartTwoHandGrab();
		}
	}
	else
	{
		RefreshGrabStat(GrabSourceComponent);
	}
}

void UIF_GrabTargetComponent::BeRelease_Implementation()
{
	bIsGrabing = false;
	bIsGrabed = false;
	GrabStat = EIF_GrabStat::None;
	if (bAttachAfterGrab)
	{
		bIsAttached = false;
		GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
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
	if (RefreshGrabStat(SourceComponent))
	{
		OutStat = GrabStat;
		return true;
	}
	
	return false;
}

PRAGMA_ENABLE_OPTIMIZATION