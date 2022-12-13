// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_DragComponent.h"

PRAGMA_DISABLE_OPTIMIZATION
// Sets default values for this component's properties
UIF_DragComponent::UIF_DragComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIF_DragComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UIF_DragComponent::UpdateDrag(float Delta)
{
	if (!DragSourceComponent)
	{
		return;
	}
	const FTransform NewSource = DragSourceComponent->GetComponentTransform();
	const FTransform DragCompTrans = GetComponentTransform();
	switch (SourceType)
	{
	case EIF_DragType::Linear_X:
		{
			float Offset = 0;
			if (bSourceInTargetSpace)
			{
				Offset = (DragCompTrans.InverseTransformPosition(NewSource.GetLocation()) -  DragCompTrans.InverseTransformPosition(SourceComponentTransform.GetLocation())).X * Scale;
			}
			else
			{
				Offset = (NewSource.GetLocation() -  SourceComponentTransform.GetLocation()).X * Scale;
			}
			SetDragOffset(Offset);
			break;
		}
	case EIF_DragType::Linear_Y:
		{
			float Offset = 0;
			if (bSourceInTargetSpace)
			{
				Offset = (DragCompTrans.InverseTransformPosition(NewSource.GetLocation()) -  DragCompTrans.InverseTransformPosition(SourceComponentTransform.GetLocation())).Y * Scale;
			}
			else
			{
				Offset  = (NewSource.GetLocation() -  SourceComponentTransform.GetLocation()).Y * Scale;
			}
			SetDragOffset(Offset);
			break;
		}
	case EIF_DragType::Linear_Z:
		{
			float Offset = 0;
			if (bSourceInTargetSpace)
			{
				Offset = (DragCompTrans.InverseTransformPosition(NewSource.GetLocation()) -  DragCompTrans.InverseTransformPosition(SourceComponentTransform.GetLocation())).Z * Scale;
			}
			else
			{
				Offset  = (NewSource.GetLocation() -  SourceComponentTransform.GetLocation()).Z * Scale;
			}
			 
			SetDragOffset(Offset);
			break;
		}
	case EIF_DragType::Rotation_X:
		{
			break;
		}
	case EIF_DragType::Rotation_Y:
		{
			break;
		}
	case EIF_DragType::Rotation_Z:
		{
			break;
		}
	case EIF_DragType::Free:
		{
			break;
		}
	case EIF_DragType::Angle_X:
		{
			break;
		}
	case EIF_DragType::Angle_Y:
		{
			break;
		}
	case EIF_DragType::Angle_Z:
		{
			break;
		}
		default:break;
	}
}

void UIF_DragComponent::SetDragOffset(float Offset)
{
	if (Offset == 0)
	{
		return;
	}
	FTransform TargetTransform = DragComponentTransform;
	switch (TargetType)
	{
	case EIF_DragType::Linear_X:
		{
			if (bTargetTypeIsWorldSpace)
			{
				TargetTransform.SetLocation(FVector(TargetTransform.GetLocation().X + Offset, TargetTransform.GetLocation().Y,TargetTransform.GetLocation().Z));
			}
			else
			{
				FVector Location = GetForwardVector() * Offset + TargetTransform.GetLocation();
				TargetTransform.SetLocation(Location);
			}
			break;
		}
	case EIF_DragType::Linear_Y:
		{
			if (bTargetTypeIsWorldSpace)
			{
				TargetTransform.SetLocation(FVector(TargetTransform.GetLocation().X , TargetTransform.GetLocation().Y+ Offset,TargetTransform.GetLocation().Z));
			}
			else
			{
				FVector Location = GetRightVector() * Offset + TargetTransform.GetLocation();
				TargetTransform.SetLocation(Location);
			}
			break;
		}
	case EIF_DragType::Linear_Z:
		{
			if (bTargetTypeIsWorldSpace)
			{
				TargetTransform.SetLocation(FVector(TargetTransform.GetLocation().X , TargetTransform.GetLocation().Y,TargetTransform.GetLocation().Z + Offset));
			}
			else
			{
				FVector Location = GetUpVector() * Offset + TargetTransform.GetLocation();
				TargetTransform.SetLocation(Location);
			}
			break;
		}
	case EIF_DragType::Rotation_X:
		{
			break;
		}
	case EIF_DragType::Rotation_Y:
		{
			break;
		}
	case EIF_DragType::Rotation_Z:
		{
			break;
		}
	case EIF_DragType::Free:
		{
			break;
		}
	case EIF_DragType::Angle_X:
		{
			break;
		}
	case EIF_DragType::Angle_Y:
		{
			break;
		}
	case EIF_DragType::Angle_Z:
		{
			break;
		}
	default:break;
	}
	
	SetWorldTransform(TargetTransform);
}


// Called every frame
void UIF_DragComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsWorking())
	{
		return;
	}
	UpdateDrag(DeltaTime);
	
	
}

bool UIF_DragComponent::StartDrag(USceneComponent* SourceComponent)
{
	if (!SourceComponent || !IsWorking())
	{
		return false;
	}
	DragSourceComponent = SourceComponent;
	bIsDraging = true;
	SourceComponentTransform = SourceComponent->GetComponentTransform();
	DragComponentTransform = GetComponentTransform();
	return true;
}

void UIF_DragComponent::StopDrag(USceneComponent* SourceComponent)
{
	if (!SourceComponent || !IsWorking())
	{
		return;
	}
	DragSourceComponent = nullptr;
	bIsDraging = false;
}



PRAGMA_ENABLE_OPTIMIZATION