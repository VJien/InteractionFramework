// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_VR_MoveComponent.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UIF_VR_MoveComponent::UIF_VR_MoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIF_VR_MoveComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}


// Called every frame
void UIF_VR_MoveComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UIF_VR_MoveComponent::IsValidTeleportLocation(const FHitResult& HitResult, FVector& ProjectLocation)
{
	bool bProjectSuccess = UNavigationSystemV1::K2_ProjectPointToNavigation(this,HitResult.ImpactPoint,
		ProjectLocation, nullptr,nullptr,TeleportProjectNavigationExtent);
	return bProjectSuccess&& HitResult.bBlockingHit;
}

inline void UIF_VR_MoveComponent::Init_Implementation(FIF_VRMoveData InitData)
{

	if (!TraceVisual)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = VR_Pawn;
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		TraceVisual = GetWorld()->SpawnActor<AIF_VR_TraceVisual>(AIF_VR_TraceVisual::StaticClass(),GetOwner()->GetActorTransform(), SpawnParameters);
	}
	VR_Pawn =InitData.VR_Pawn;
	MovementComponent = InitData.MovementComponent;
	TraceAimComponent = InitData.TraceAimComponent;
	Camera = InitData.Camera;
	
}

void UIF_VR_MoveComponent::StartTeleportTrace_Implementation()
{
	if (TraceVisual)
	{
		TraceVisual->Show();
	}
	bTeleportTraceActive = true;
}

void UIF_VR_MoveComponent::UpdateTeleportTrace_Implementation()
{
	if (!TraceAimComponent)
	{
		return;
	}
	const FVector StartPos = TraceAimComponent->GetComponentLocation();
	const FVector FwdVec = TraceAimComponent->GetForwardVector();
	FHitResult Hit;
	switch (TeleportTraceType)
	{
	case  EIF_VRTraceType::Parabola:
		{
			TeleportPathPoints.Empty();
			
			FVector LastPoint;
			if (bTeleportTraceByChannel)
			{
				UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(this,Hit,TeleportPathPoints,LastPoint,StartPos,FwdVec* TeleportLaunchSpeed,
					true,TeleportProjectileRaidus, UEngineTypes::ConvertToCollisionChannel(TeleportChannel),false,TeleportIgnoreActors,TeleportDrawDebugType,TeleportDrawDebugTime,
					TeleportSimFrequency,TeleportMaxSimTime, TeleportOverrideGravityZ);
			}
			else
			{
				UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(this,Hit,TeleportPathPoints,LastPoint,StartPos,FwdVec* TeleportLaunchSpeed,
					true,TeleportProjectileRaidus,TeleportObjectTypes,false,TeleportIgnoreActors,TeleportDrawDebugType,TeleportDrawDebugTime,
					TeleportSimFrequency,TeleportMaxSimTime, TeleportOverrideGravityZ);
			}
			TeleportPathPoints.Insert(StartPos,0);
			bValidTeleportLocation = IsValidTeleportLocation(Hit,TeleportProjectedLocation);
			TeleportProjectedLocation.Z += TeleportProjectNavMeshCellHeight;
			if (TraceVisual)
			{
				TraceVisual->UpdateData(TeleportProjectedLocation, TeleportPathPoints, bValidTeleportLocation);
			}
			
		}
	case EIF_VRTraceType::Linear:
		{
			TeleportPathPoints.Empty();
			FVector LastPoint;
			if (bTeleportTraceByChannel)
			{
				UKismetSystemLibrary::LineTraceSingle(this,StartPos, FwdVec * TeleportTraceDistance + StartPos, TeleportChannel,false, TeleportIgnoreActors,TeleportDrawDebugType,
					Hit,false);
			}
			else
			{
				UKismetSystemLibrary::LineTraceSingleForObjects(this,StartPos, FwdVec * TeleportTraceDistance + StartPos, TeleportObjectTypes,false, TeleportIgnoreActors,TeleportDrawDebugType,
					Hit,false);
			}
			//todo 处理max distance
			FVector PawnLoc = VR_Pawn->GetActorLocation();
			PawnLoc.Z -= VR_Pawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			if ((Hit.ImpactPoint - PawnLoc).Size()> TeleportMaxDistance)
			{
				FVector Dir = Hit.ImpactPoint - StartPos;
				Dir.Z = 0;
				Dir.Normalize();
				Hit.ImpactPoint =   StartPos + Dir * TeleportMaxDistance;
				if (TeleportDrawDebugType != EDrawDebugTrace::None)
				{
					UKismetSystemLibrary::DrawDebugCapsule(this,Hit.ImpactPoint,88,40,FRotator::ZeroRotator,FLinearColor::Green,5,1);
					UKismetSystemLibrary::DrawDebugLine(this,PawnLoc,Hit.ImpactPoint,FLinearColor::Green,5,1);
				}
			}
			
			bValidTeleportLocation = IsValidTeleportLocation(Hit,TeleportProjectedLocation);
			TeleportPathPoints.Emplace(StartPos);
			TeleportPathPoints.Emplace(TeleportProjectedLocation);
			TeleportProjectedLocation.Z += TeleportProjectNavMeshCellHeight;
			
			
			if (TraceVisual)
			{
				TraceVisual->UpdateData(TeleportProjectedLocation, TeleportPathPoints, bValidTeleportLocation);
			}
		}
		default:
			break;
	}
}



void UIF_VR_MoveComponent::FinishAndTryTeleport_Implementation()
{
	if (TraceVisual)
	{
		TraceVisual->Hidden();
	}
	bTeleportTraceActive = false;
	if (bValidTeleportLocation)
	{
		VR_Pawn->TeleportTo(TeleportProjectedLocation/* + VR_Pawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*/, VR_Pawn->GetActorRotation());
		bValidTeleportLocation = false;
	}
}

void UIF_VR_MoveComponent::AdjustCapsule_Implementation()
{
}

void UIF_VR_MoveComponent::MoveCharacter_Implementation(float Fwd, float Right)
{
	if (!VR_Pawn || !MovementComponent)
	{
		return;
	}
	
	switch (MoveDirection)
	{
	case EIF_VRMoveDirection::Camera:
		{
			FVector Direction =Camera->GetForwardVector() * Fwd + Camera->GetRightVector() * Right;
			if (bKeepHorizonDirection)
			{
				Direction.Z = 0;
				Direction.Normalize();
			}
			VR_Pawn->AddMovementInput(Direction, MoveSpeedScale);
		}
	case EIF_VRMoveDirection::CustomAimComponent:
		{
			if (!TraceAimComponent)
			{
				return;
			}
			FVector Direction = TraceAimComponent->GetForwardVector() * Fwd + TraceAimComponent->GetRightVector() * Right;
			if (bKeepHorizonDirection)
			{
				Direction.Z = 0;
				Direction.Normalize();
			}
			VR_Pawn->AddMovementInput(Direction, MoveSpeedScale);
		}
		default:
			break;
	}
}

