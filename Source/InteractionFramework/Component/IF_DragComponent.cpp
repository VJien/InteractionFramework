// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_DragComponent.h"

#include "Kismet/KismetMathLibrary.h"

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
	CurrOffset = 0;
	// ...
	
}

void UIF_DragComponent::UpdateDrag(float Delta)
{
	if (!DragSourceComponent)
	{
		return;
	}
	const FTransform NewSourceTM = DragSourceComponent->GetComponentTransform();
	const FTransform NewDragTM = GetComponentTransform(); 
	float Offset = 0;
	switch (SourceType)
	{
	case EIF_DragType_Source::Linear_X:
		{
			if (bSourceInTargetSpace)
			{
				Offset = (NewDragTM.InverseTransformPosition(NewSourceTM.GetLocation()) -  NewDragTM.InverseTransformPosition(SourceComponentStartTM.GetLocation())).X * Scale;
			}
			else
			{
				Offset = (NewSourceTM.GetLocation() -  SourceComponentStartTM.GetLocation()).X * Scale;
			}
			SetDragOffset(Offset);
			break;
		}
	case EIF_DragType_Source::Linear_Y:
		{
			if (bSourceInTargetSpace)
			{
				Offset = (NewDragTM.InverseTransformPosition(NewSourceTM.GetLocation()) -  NewDragTM.InverseTransformPosition(SourceComponentStartTM.GetLocation())).Y * Scale;
			}
			else
			{
				Offset  = (NewSourceTM.GetLocation() -  SourceComponentStartTM.GetLocation()).Y * Scale;
			}
			SetDragOffset(Offset);
			break;
		}
	case EIF_DragType_Source::Linear_Z:
		{
			if (bSourceInTargetSpace)
			{
				Offset = (NewDragTM.InverseTransformPosition(NewSourceTM.GetLocation()) -  NewDragTM.InverseTransformPosition(SourceComponentStartTM.GetLocation())).Z * Scale;
			}
			else
			{
				Offset  = (NewSourceTM.GetLocation() -  SourceComponentStartTM.GetLocation()).Z * Scale;
			}
			 
			SetDragOffset(Offset);
			break;
		}
	case EIF_DragType_Source::Rotation_Roll:
		{
			if (bSourceInTargetSpace)
			{
				Offset =UKismetMathLibrary::NormalizedDeltaRotator(NewDragTM.InverseTransformRotation(NewSourceTM.GetRotation()).Rotator(),
					NewDragTM.InverseTransformRotation(SourceComponentStartTM.GetRotation()).Rotator()).Roll * Scale;
			}
			else
			{ 
				Offset  = UKismetMathLibrary::NormalizedDeltaRotator(NewSourceTM.GetRotation().Rotator() , SourceComponentStartTM.GetRotation().Rotator()).Roll * Scale;
			}
			 
			SetDragOffset(Offset);

			break;
		}
	case EIF_DragType_Source::Rotation_Pitch:
		{
			if (bSourceInTargetSpace)
			{
				Offset =UKismetMathLibrary::NormalizedDeltaRotator(NewDragTM.InverseTransformRotation(NewSourceTM.GetRotation()).Rotator(),
					NewDragTM.InverseTransformRotation(SourceComponentStartTM.GetRotation()).Rotator()).Pitch * Scale;
			}
			else
			{ 
				Offset  = UKismetMathLibrary::NormalizedDeltaRotator(NewSourceTM.GetRotation().Rotator() , SourceComponentStartTM.GetRotation().Rotator()).Pitch * Scale;
			}
			 
			SetDragOffset(Offset);

			break;
		}
	case EIF_DragType_Source::Rotation_Yaw:
		{
			if (bSourceInTargetSpace)
			{
				Offset =UKismetMathLibrary::NormalizedDeltaRotator(NewDragTM.InverseTransformRotation(NewSourceTM.GetRotation()).Rotator(),
					NewDragTM.InverseTransformRotation(SourceComponentStartTM.GetRotation()).Rotator()).Yaw * Scale;
			}
			else
			{ 
				Offset  = UKismetMathLibrary::NormalizedDeltaRotator(NewSourceTM.GetRotation().Rotator() , SourceComponentStartTM.GetRotation().Rotator()).Yaw * Scale;
			}
			 
			SetDragOffset(Offset);

			break;
		}
	case EIF_DragType_Source::Angle_Roll:
		{
			FTransform SpecialTransform;
			SpecialTransform.SetLocation(NewDragTM.GetLocation());
			FVector RollDir = (NewSourceTM.GetLocation() - NewDragTM.GetLocation());
			RollDir = FVector::VectorPlaneProject(RollDir,NewDragTM.GetRotation().GetForwardVector());
			FRotator SpecialRot = UKismetMathLibrary::MakeRotFromXY(NewDragTM.GetRotation().GetForwardVector(), RollDir);
			SpecialTransform.SetRotation(SpecialRot.Quaternion());

			FTransform TargetTransform = RelativeTM_Roll.Inverse() * SpecialTransform;
			FRotator TargetRot = NewDragTM.GetRotation().Rotator();
			TargetRot.Roll = TargetTransform.GetRotation().Rotator().Roll;
			if (bClamp)
			{
				TargetRot.Roll = FMath::Clamp(TargetRot.Roll, DragComponentStartTM.GetRotation().Rotator().Roll + Min, DragComponentStartTM.GetRotation().Rotator().Roll + Max);
			}
			SetWorldRotation(TargetRot);
			break;
		}
	case EIF_DragType_Source::Angle_Pitch:
		{
			FTransform SpecialTransform;
			SpecialTransform.SetLocation(NewDragTM.GetLocation());

			FVector PitchDir = (NewSourceTM.GetLocation() - NewDragTM.GetLocation());
			PitchDir =  FVector::VectorPlaneProject(PitchDir, NewDragTM.GetRotation().GetRightVector());
			
			FRotator SpecialRot = UKismetMathLibrary::MakeRotFromXY(PitchDir.GetSafeNormal(), NewDragTM.GetRotation().GetRightVector() );
			SpecialTransform.SetRotation(SpecialRot.Quaternion());
			FTransform TargetTransform = RelativeTM_Pitch.Inverse() * SpecialTransform;
			FRotator TargetRot = NewDragTM.GetRotation().Rotator();
			TargetRot.Pitch = TargetTransform.GetRotation().Rotator().Pitch;
			TargetRot.Normalize();
			if (bClamp)
			{
				TargetRot.Pitch = FMath::Clamp(TargetRot.Pitch,Min,Max);
			}
			SetWorldRotation(TargetRot);
			break;
		}
	case EIF_DragType_Source::Angle_Yaw:
		{
			FTransform SpecialTransform;
			SpecialTransform.SetLocation(NewDragTM.GetLocation());
			FVector YawDir = (NewSourceTM.GetLocation() - NewDragTM.GetLocation());
			YawDir.Z = 0;
			FRotator SpecialRot = UKismetMathLibrary::MakeRotFromYZ( YawDir.GetSafeNormal(),NewDragTM.GetRotation().GetUpVector() * -1);
			SpecialTransform.SetRotation(SpecialRot.Quaternion());
			
			FTransform TargetTransform = RelativeTM_Yaw * SpecialTransform;
			FRotator TargetRot = NewDragTM.GetRotation().Rotator();
			TargetRot.Yaw = TargetTransform.GetRotation().Rotator().Yaw;
			if (bClamp)
			{
				TargetRot.Yaw = FMath::Clamp(TargetRot.Yaw,DragComponentStartTM.GetRotation().Rotator().Yaw+Min,DragComponentStartTM.GetRotation().Rotator().Yaw+Max);
			}
			
			SetWorldRotation(TargetRot);
			break;
		}
	case EIF_DragType_Source::Linear:
		{
			FTransform TargetTransform = RelativeTM.Inverse() * NewSourceTM;
			SetWorldLocation(TargetTransform.GetLocation());
			break;
		}
	case EIF_DragType_Source::Rotation:
		{
			FTransform TargetTransform = RelativeTM.Inverse() * NewSourceTM;
			SetWorldRotation(TargetTransform.GetRotation());
			break;
		}
	case EIF_DragType_Source::Angle:
		{
			
			FTransform SpecialTransform;
			SpecialTransform.SetLocation(NewDragTM.GetLocation());
			FVector Dir = (NewSourceTM.GetLocation() - NewDragTM.GetLocation());
			FRotator SpecialRot = UKismetMathLibrary::MakeRotFromX( Dir.GetSafeNormal());
			SpecialTransform.SetRotation(SpecialRot.Quaternion());
			
			FTransform TargetTransform = RelativeTM_Angle.Inverse() * SpecialTransform;
			FRotator TargetRot = TargetTransform.GetRotation().Rotator();
			SetWorldRotation(TargetRot);
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
	float Percent = 1;
	if (bClamp)
	{
		Offset = FMath::Clamp(Offset, Min - SavedOffset, Max - SavedOffset);
		CurrOffset = Offset + SavedOffset ;
		Percent = FMath::Clamp((Offset - (Min - SavedOffset)) / (Max - Min),0.f,1.f);
	}
	FTransform TargetTransform = DragComponentStartTM;
	switch (TargetType)
	{
	case EIF_DragType_Target::Linear_X:
		{
			if (bTargetTypeIsWorldSpace)
			{
				TargetTransform.SetLocation(FVector(TargetTransform.GetLocation().X + Offset, TargetTransform.GetLocation().Y,TargetTransform.GetLocation().Z));
			}
			else
			{
				FTransform CurrTM = DragComponentStartRelativeTM * DragSourceParentComponent->GetComponentTransform();
				FVector Location = CurrTM.GetRotation().Vector() * Offset + CurrTM.GetLocation();
				TargetTransform.SetLocation(Location);
				TargetTransform.SetRotation(CurrTM.GetRotation());
			}
			OnDragTransition.Broadcast(Percent, EIF_DragType_Target::Linear_X);
			break;
		}
	case EIF_DragType_Target::Linear_Y:
		{
			if (bTargetTypeIsWorldSpace)
			{
				TargetTransform.SetLocation(FVector(TargetTransform.GetLocation().X , TargetTransform.GetLocation().Y+ Offset,TargetTransform.GetLocation().Z));
			}
			else
			{
				FTransform CurrTM = DragComponentStartRelativeTM * DragSourceParentComponent->GetComponentTransform();
				FVector Location = UKismetMathLibrary::GetRightVector(CurrTM.Rotator()) * Offset + CurrTM.GetLocation();
				TargetTransform.SetLocation(Location);
				TargetTransform.SetRotation(CurrTM.GetRotation());
			}
			OnDragTransition.Broadcast(Percent, EIF_DragType_Target::Linear_Y);
			break;
		}
	case EIF_DragType_Target::Linear_Z:
		{
			if (bTargetTypeIsWorldSpace)
			{
				TargetTransform.SetLocation(FVector(TargetTransform.GetLocation().X , TargetTransform.GetLocation().Y,TargetTransform.GetLocation().Z + Offset));
			}
			else
			{
				FTransform CurrTM = DragComponentStartRelativeTM * DragSourceParentComponent->GetComponentTransform();
				FVector Location = UKismetMathLibrary::GetUpVector(CurrTM.Rotator()) * Offset + CurrTM.GetLocation();
				TargetTransform.SetLocation(Location);
				TargetTransform.SetRotation(CurrTM.GetRotation());
			}
			OnDragTransition.Broadcast(Percent, EIF_DragType_Target::Linear_Z);
			break;
		}
	case EIF_DragType_Target::Rotation_Roll:
		{
			FRotator Rot = TargetTransform.GetRotation().Rotator();
			Rot.Roll += Offset;
			TargetTransform.SetRotation(Rot.Quaternion());
			
			break;
		}
	case EIF_DragType_Target::Rotation_Pitch:
		{
			FRotator Rot = TargetTransform.GetRotation().Rotator();
			Rot.Pitch += Offset;
			TargetTransform.SetRotation(Rot.Quaternion());
			break;
		}
	case EIF_DragType_Target::Rotation_Yaw:
		{
			FRotator Rot = TargetTransform.GetRotation().Rotator();
			Rot.Yaw += Offset;
			TargetTransform.SetRotation(Rot.Quaternion());
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
	SourceComponentStartTM = SourceComponent->GetComponentTransform();
	DragComponentStartTM = GetComponentTransform();
	DragSourceParentComponent = GetAttachParent();
	DragComponentStartRelativeTM = GetRelativeTransform();
	FTransform SpecialTransform;
	SpecialTransform.SetLocation(DragComponentStartTM.GetLocation());
	FVector RollDir = (SourceComponentStartTM.GetLocation() - DragComponentStartTM.GetLocation());
	RollDir = FVector::VectorPlaneProject(RollDir,DragComponentStartTM.GetRotation().GetForwardVector());
	FRotator SpecialRot = UKismetMathLibrary::MakeRotFromXY(DragComponentStartTM.GetRotation().GetForwardVector(), RollDir.GetSafeNormal());
	SpecialTransform.SetRotation(SpecialRot.Quaternion());
	RelativeTM_Roll = SpecialTransform.GetRelativeTransform(DragComponentStartTM);

	FVector PitchDir = (SourceComponentStartTM.GetLocation() - DragComponentStartTM.GetLocation());
	PitchDir = FVector::VectorPlaneProject(PitchDir, DragComponentStartTM.GetRotation().GetRightVector());
	SpecialRot = UKismetMathLibrary::MakeRotFromXY( PitchDir.GetSafeNormal(), DragComponentStartTM.GetRotation().GetRightVector());
	SpecialTransform.SetRotation(SpecialRot.Quaternion());
	RelativeTM_Pitch = SpecialTransform.GetRelativeTransform(DragComponentStartTM);

	FVector YawDir = (SourceComponentStartTM.GetLocation() - DragComponentStartTM.GetLocation());
	YawDir.Z = 0;
	SpecialRot = UKismetMathLibrary::MakeRotFromYZ( YawDir.GetSafeNormal(),SourceComponentStartTM.GetRotation().GetUpVector() * -1);
	SpecialTransform.SetRotation(SpecialRot.Quaternion());
	RelativeTM_Yaw = SpecialTransform.GetRelativeTransform(DragComponentStartTM);

	FVector Dir = (SourceComponentStartTM.GetLocation() - DragComponentStartTM.GetLocation());
	SpecialRot = UKismetMathLibrary::MakeRotFromX( Dir.GetSafeNormal());
	SpecialTransform.SetRotation(SpecialRot.Quaternion());
	RelativeTM_Angle =  SpecialTransform.GetRelativeTransform(DragComponentStartTM);

	
	RelativeTM = SourceComponentStartTM.GetRelativeTransform(DragComponentStartTM);

	OnStartDrag.Broadcast(SourceComponent);
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
	SavedOffset = CurrOffset;
	CurrOffset = 0;
	OnStopDrag.Broadcast(SourceComponent);
}



PRAGMA_ENABLE_OPTIMIZATION