// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_VR_TargetCheckComponent.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"


AActor* UIF_TargetCheckConfig::GetActor_Implementation(USceneComponent* SourceCompennt)
{
	return nullptr;
}

FHitResult UIF_TargetCheckConfig::GetHitResult_Implementation(USceneComponent* SourceCompennt)
{
	return FHitResult();
}

bool UIF_TargetCheckConfig::DoesHitResultMatched(const FHitResult& HitResult)
{
	if (!bEnableFilter)
	{
		return true;
	}
	if (!HitResult.IsValidBlockingHit() || HitResult.GetActor() == nullptr)
	{
		return false;
	}
	if (ClassOnlyCheck != nullptr && !HitResult.GetActor()->GetClass()->IsChildOf(ClassOnlyCheck))
	{
		return false;
	}
	if (ClassesIgnore.Contains(HitResult.GetActor()->GetClass()))
	{
		return false;
	}
	for (auto C: ClassesIgnore)
	{
		if (HitResult.GetActor()->GetClass()->IsChildOf(C))
		{
			return false;
		}
	}
	const auto Tags = HitResult.GetActor()->Tags;
	if (ActorWithTags.Num() > 0)
	{
		for (auto Tag : ActorWithTags)
		{
			if (!Tags.Contains(Tag))
			{
				return false;
			}
		}
	}
	if (ActorWithoutTags.Num() > 0)
	{
		for (auto Tag : ActorWithoutTags)
		{
			if (Tags.Contains(Tag))
			{
				return false;
			}
		}
	}
	return true;
	
}

bool UIF_TargetCheckConfig::DoesActorMatched(AActor* Actor)
{
	if (!bEnableFilter)
	{
		return true;
	}
	if (Actor == nullptr)
	{
		return false;
	}
	// // 获取Actor的时候已经过滤过一遍了. 这里不用了
	// if (ClassOnlyCheck != nullptr && !Actor->GetClass()->IsChildOf(ClassOnlyCheck))
	// {
	// 	return false;
	// }
	if (ClassesIgnore.Contains(Actor->GetClass()))
	{
		return false;
	}
	for (auto C: ClassesIgnore)
	{
		if (Actor->GetClass()->IsChildOf(C))
		{
			return false;
		}
	}
	const auto Tags =Actor->Tags;
	if (ActorWithTags.Num() > 0)
	{
		for (auto Tag : ActorWithTags)
		{
			if (Tag ==NAME_None)
			{
				continue;
			}
			if (!Tags.Contains(Tag))
			{
				return false;
			}
		}
	}
	if (ActorWithoutTags.Num() > 0)
	{
		for (auto Tag : ActorWithoutTags)
		{
			if (Tag ==NAME_None)
			{
				continue;
			}
			if (Tags.Contains(Tag))
			{
				return false;
			}
		}
	}
	return true;
}

void UIF_TargetCheckConfig::AddIgnoreActors(const TArray<AActor*> List)
{
	IgnoreActors.Append(List);
}

void UIF_TargetCheckConfig::RemoveIgnoreActor(AActor* RemoveActor)
{
	IgnoreActors.Remove(RemoveActor);
}

void UIF_TargetCheckConfig::RemoveIgnoreActors(const TArray<AActor*> List)
{
	for (auto i: List)
	{
		IgnoreActors.Remove(i);
	}
}

void UIF_TargetCheckConfig::ClearIgnoreActors()
{
	IgnoreActors.Empty();
}

FHitResult UIF_TargetCheckConfig_TraceBase::GetBestMultiHitResult(USceneComponent* SourceCompennt, const TArray<FHitResult> HitResults)
{
	if (HitResults.Num()==0 || SourceCompennt==nullptr)
	{
		return FHitResult();
	}
	if (HitResults.Num()==1)
	{
		if (DoesHitResultMatched( HitResults[0]))
		{
			return HitResults[0];
		}
		return FHitResult();
	}
	FHitResult BestHit;
	switch (MultiCheckRule)
	{
		
	case EIF_TargetCheckMultiResultRule::Default:
		{
			for (auto Hit: HitResults)
			{
				if (DoesHitResultMatched( Hit))
				{
					return Hit;
				}
			}
			return BestHit;
		}
	case EIF_TargetCheckMultiResultRule::Near:
		{
			float Dist = FLT_MAX;
			for (auto P: HitResults)
			{
				if (!DoesHitResultMatched(P))
				{
					continue;
				}
				const float NewDist = (SourceCompennt->GetComponentLocation() - P.ImpactPoint).SizeSquared();
				if (NewDist < Dist)
				{
					Dist = NewDist;
					BestHit = P;
				}
			}
			return BestHit;
		}
	case EIF_TargetCheckMultiResultRule::Far:
		{
			float Dist = FLT_MIN;
			for (auto P: HitResults)
			{
				if (!DoesHitResultMatched(P))
				{
					continue;
				}
				const float NewDist = (SourceCompennt->GetComponentLocation() - P.ImpactPoint).SizeSquared();
				if (NewDist > Dist)
				{
					Dist = NewDist;
					BestHit = P;
				}
			}
			return BestHit;
		}

		default:
			break;
	}
	return BestHit;
}

AActor* UIF_TargetCheckConfig_Sphere::GetActor_Implementation(USceneComponent* SourceCompennt)
{
	if (!SourceCompennt)
	{
		return nullptr;
	}
	FVector Start = SourceCompennt->GetComponentLocation();
	FVector Dir = SourceCompennt->GetForwardVector();
	if (bMultiTrace)
	{

		TArray<FHitResult> Results;
		if (bUseChannel)
		{
			if (UKismetSystemLibrary::SphereTraceMulti(SourceCompennt,Start, Start + Dir*Distance,Radius, Channel, false, IgnoreActors, DebugType,Results,bIgnoreOwner))
			{
				return GetBestMultiHitResult(SourceCompennt, Results).GetActor();
			}
		
		}
		else
		{
			if (UKismetSystemLibrary::SphereTraceMultiForObjects(SourceCompennt,Start, Start + Dir*Distance,Radius, Collision, false, IgnoreActors, DebugType,Results,bIgnoreOwner))
			{
				return GetBestMultiHitResult(SourceCompennt, Results).GetActor();
			}
		}
		
	}
	else
	{
		FHitResult Result;
		if (bUseChannel)
		{
			if (UKismetSystemLibrary::SphereTraceSingle(SourceCompennt,Start, Start + Dir*Distance,Radius, Channel, false, IgnoreActors, DebugType,Result,bIgnoreOwner))
			{
				if (DoesHitResultMatched(Result))
				{
					return Result.GetActor();
				}
				return nullptr;
			}
		
		}
		else
		{
			if (UKismetSystemLibrary::SphereTraceSingleForObjects(SourceCompennt,Start, Start + Dir*Distance,Radius, Collision, false, IgnoreActors, DebugType,Result,bIgnoreOwner))
			{
				if (DoesHitResultMatched(Result))
				{
					return Result.GetActor();
				}
				return nullptr;
			}
		}
	}
	return nullptr;
}

FHitResult UIF_TargetCheckConfig_Sphere::GetHitResult_Implementation(USceneComponent* SourceCompennt)
{
	FHitResult Result;
	if (!SourceCompennt)
	{
		return Result;
	}
	FVector Start = SourceCompennt->GetComponentLocation();
	FVector Dir = SourceCompennt->GetForwardVector();
	if (bMultiTrace)
	{
	
		TArray<FHitResult> Results;
		if (bUseChannel)
		{
			if (UKismetSystemLibrary::SphereTraceMulti(SourceCompennt,Start, Start + Dir*Distance,Radius, Channel, false, IgnoreActors, DebugType,Results,bIgnoreOwner))
			{
				return GetBestMultiHitResult(SourceCompennt, Results);
			}
		
		}
		else
		{
			if (UKismetSystemLibrary::SphereTraceMultiForObjects(SourceCompennt,Start, Start + Dir*Distance,Radius, Collision, false, IgnoreActors, DebugType,Results,bIgnoreOwner))
			{
				return GetBestMultiHitResult(SourceCompennt, Results);
			}
		}
		
	}
	else
	{
		if (bUseChannel)
		{
			if (UKismetSystemLibrary::SphereTraceSingle(SourceCompennt,Start, Start + Dir*Distance,Radius, Channel, false, IgnoreActors, DebugType,Result,bIgnoreOwner))
			{
				return Result;
			}
		
		}
		else
		{
			if (UKismetSystemLibrary::SphereTraceSingleForObjects(SourceCompennt,Start, Start + Dir*Distance,Radius, Collision, false, IgnoreActors, DebugType,Result,bIgnoreOwner))
			{
				return Result;
			}
		}
	}
	return Result;
}

AActor* UIF_TargetCheckConfig_Box::GetActor_Implementation(USceneComponent* SourceCompennt)
{
	if (!SourceCompennt)
	{
		return nullptr;
	}
	FVector Start = SourceCompennt->GetComponentLocation();
	FVector Dir = SourceCompennt->GetForwardVector();
	if (bMultiTrace)
	{

		TArray<FHitResult> Results;
		if (bUseChannel)
		{
			if (UKismetSystemLibrary::BoxTraceMulti(SourceCompennt,Start, Start + Dir*Distance,Extend,SourceCompennt->GetForwardVector().Rotation(), Channel, false, IgnoreActors, DebugType,Results,bIgnoreOwner))
			{
				return GetBestMultiHitResult(SourceCompennt, Results).GetActor();
			}
		
		}
		else
		{
			if (UKismetSystemLibrary::BoxTraceMultiForObjects(SourceCompennt,Start, Start + Dir*Distance,Extend,SourceCompennt->GetForwardVector().Rotation(), Collision, false, IgnoreActors, DebugType,Results,bIgnoreOwner))
			{
				return GetBestMultiHitResult(SourceCompennt, Results).GetActor();
			}
		}
		
	}
	else
	{
		FHitResult Result;
		if (bUseChannel)
		{
			if (UKismetSystemLibrary::BoxTraceSingle(SourceCompennt,Start, Start + Dir*Distance,Extend,SourceCompennt->GetForwardVector().Rotation(), Channel, false, IgnoreActors, DebugType,Result,bIgnoreOwner))
			{
				if (DoesHitResultMatched(Result))
				{
					return Result.GetActor();
				}
				return nullptr;
			}
		
		}
		else
		{
			if (UKismetSystemLibrary::BoxTraceSingleForObjects(SourceCompennt,Start, Start + Dir*Distance,Extend,SourceCompennt->GetForwardVector().Rotation(), Collision, false, IgnoreActors, DebugType,Result,bIgnoreOwner))
			{
				if (DoesHitResultMatched(Result))
				{
					return Result.GetActor();
				}
				return nullptr;
			}
		}
	}
	return nullptr;
}

FHitResult UIF_TargetCheckConfig_Box::GetHitResult_Implementation(USceneComponent* SourceCompennt)
{
	if (!SourceCompennt)
	{
		return  FHitResult();;
	}
	FVector Start = SourceCompennt->GetComponentLocation();
	FVector Dir = SourceCompennt->GetForwardVector();
	if (bMultiTrace)
	{

		TArray<FHitResult> Results;
		if (bUseChannel)
		{
			if (UKismetSystemLibrary::BoxTraceMulti(SourceCompennt,Start, Start + Dir*Distance,Extend,SourceCompennt->GetForwardVector().Rotation(), Channel, false, IgnoreActors, DebugType,Results,bIgnoreOwner))
			{
				return GetBestMultiHitResult(SourceCompennt, Results);
			}
		
		}
		else
		{
			if (UKismetSystemLibrary::BoxTraceMultiForObjects(SourceCompennt,Start, Start + Dir*Distance,Extend,SourceCompennt->GetForwardVector().Rotation(), Collision, false, IgnoreActors, DebugType,Results,bIgnoreOwner))
			{
				return GetBestMultiHitResult(SourceCompennt, Results);
			}
		}
		
	}
	else
	{
		FHitResult Result;
		if (bUseChannel)
		{
			if (UKismetSystemLibrary::BoxTraceSingle(SourceCompennt,Start, Start + Dir*Distance,Extend,SourceCompennt->GetForwardVector().Rotation(), Channel, false, IgnoreActors, DebugType,Result,bIgnoreOwner))
			{
				if (DoesHitResultMatched(Result))
				{
					return Result;
				}
				return FHitResult();
			}
		
		}
		else
		{
			if (UKismetSystemLibrary::BoxTraceSingleForObjects(SourceCompennt,Start, Start + Dir*Distance,Extend,SourceCompennt->GetForwardVector().Rotation(), Collision, false, IgnoreActors, DebugType,Result,bIgnoreOwner))
			{
				if (DoesHitResultMatched(Result))
				{
					return Result;
				}
				return FHitResult();
			}
		}
	}
	return  FHitResult();
}

UPrimitiveComponent* UIF_TargetCheckConfig_Overlap::CreateOverlapComponent(USceneComponent* SourceCompennt)
{
	return nullptr;
}

AActor* UIF_TargetCheckConfig_Overlap::GetBestActor(USceneComponent* SourceCompennt, const TArray<AActor*>& Actors)
{
	if (SourceCompennt == nullptr || Actors.Num()==0)
	{
		return nullptr;
	}

	AActor* BestActor = nullptr;
	switch (MultiCheckRule)
	{
		
	case EIF_TargetCheckMultiResultRule::Default:
		{
			for (auto Actor: Actors)
			{
				if (DoesActorMatched(Actor))
				{
					return Actor;
				}
			}
			return nullptr;
		}
	case EIF_TargetCheckMultiResultRule::Near:
		{
			float Dist = FLT_MAX;
			for (auto Actor: Actors)
			{
				if (!DoesActorMatched(Actor))
				{
					continue;
				}
				const float NewDist = (SourceCompennt->GetComponentLocation() - Actor->GetActorLocation()).SizeSquared();
				if (NewDist < Dist)
				{
					Dist = NewDist;
					BestActor = Actor;
				}
			}
			return BestActor;
		}
	case EIF_TargetCheckMultiResultRule::Far:
		{
			float Dist = FLT_MIN;
			for (auto Actor: Actors)
			{
				if (!DoesActorMatched(Actor))
				{
					continue;
				}
				const float NewDist = (SourceCompennt->GetComponentLocation() - Actor->GetActorLocation()).SizeSquared();
				if (NewDist > Dist)
				{
					Dist = NewDist;
					BestActor = Actor;
				}
			}
			return BestActor;
		}

	default:
		break;
	}
	return BestActor;

}

AActor* UIF_TargetCheckConfig_Overlap::GetActor_Implementation(USceneComponent* SourceCompennt)
{
	if (OverlapComponent == nullptr)
	{
		OverlapComponent = CreateOverlapComponent(SourceCompennt);
	}
	return nullptr;
}
UPrimitiveComponent* UIF_TargetCheckConfig_SphereOverlap::CreateOverlapComponent(USceneComponent* SourceCompennt)
{
	if (SourceCompennt)
	{
		auto Sphere = Cast<USphereComponent>(SourceCompennt->GetOwner()->AddComponentByClass(USphereComponent::StaticClass(),true, FTransform(),false));
		Sphere->SetSphereRadius(Radius);
		Sphere->SetCollisionObjectType(CollisionType);
		Sphere->AttachToComponent(SourceCompennt, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Sphere->SetGenerateOverlapEvents(true);
		OverlapComponent = Sphere;
		 
	}
	return OverlapComponent;
}

AActor* UIF_TargetCheckConfig_SphereOverlap::GetActor_Implementation(USceneComponent* SourceCompennt)
{
	Super::GetActor_Implementation(SourceCompennt);
	if (!SourceCompennt || !OverlapComponent)
	{
		return nullptr;
	}
	if (bDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugSphere(SourceCompennt->GetWorld(), OverlapComponent->GetComponentLocation(),Radius,12);
	}
	TArray<AActor*> Actors;
	OverlapComponent->GetOverlappingActors(Actors, ClassOnlyCheck);
	
	return GetBestActor(SourceCompennt, Actors);
}


UPrimitiveComponent* UIF_TargetCheckConfig_BoxOverlap::CreateOverlapComponent(USceneComponent* SourceCompennt)
{
	if (SourceCompennt)
	{
		auto Sphere = Cast<UBoxComponent>(SourceCompennt->GetOwner()->AddComponentByClass(UBoxComponent::StaticClass(),true, FTransform(),false));
		Sphere->SetBoxExtent(Extent);
		Sphere->SetCollisionObjectType(CollisionType);
		Sphere->AttachToComponent(SourceCompennt, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Sphere->SetGenerateOverlapEvents(true);
		OverlapComponent = Sphere;
		 
	}
	return OverlapComponent;
}

AActor* UIF_TargetCheckConfig_BoxOverlap::GetActor_Implementation(USceneComponent* SourceCompennt)
{
	Super::GetActor_Implementation(SourceCompennt);
	if (!SourceCompennt || !OverlapComponent)
	{
		return nullptr;
	}
	if (bDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugBox(SourceCompennt->GetWorld(), OverlapComponent->GetComponentLocation(),Extent,FColor::Green,SourceCompennt->GetForwardVector().Rotation());
	}
	TArray<AActor*> Actors;
	OverlapComponent->GetOverlappingActors(Actors,ClassOnlyCheck);
	
	return GetBestActor(SourceCompennt, Actors);
}


// Sets default values for this component's properties
UIF_TargetCheckComponent::UIF_TargetCheckComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIF_TargetCheckComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}


// Called every frame
void UIF_TargetCheckComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrTime >= CheckInterval || CheckInterval <=0)
	{
		if (Type)
		{
			AActor* NewActor = Type->GetActor(this);
			if (!(NewActor == nullptr && bRememberLastTarget) && NewActor != CurrentBestActor )
			{
				OnHitActorUpdated.Broadcast(NewActor, CurrentBestActor);
				CurrentBestActor = NewActor;
			}
		
			FHitResult NewHit = Type->GetHitResult(this);
			if ((NewHit.GetActor() != CurrentBestHit.GetActor() || NewHit.GetComponent() != CurrentBestHit.GetComponent() ||
				NewHit.ImpactPoint != CurrentBestHit.ImpactPoint) && !(!NewHit.IsValidBlockingHit() && bRememberLastTarget))
			{
				OnHandCheckHitResultUpdated.Broadcast(NewHit, CurrentBestHit);
				CurrentBestHit = NewHit;
			}
		}
		CurrTime = 0;
	}
	else
	{
		CurrTime += DeltaTime;
	}
	
	// ...
}

void UIF_TargetCheckComponent::AddIgnoreActors(const TArray<AActor*> List)
{
	if (Type)
	{
		Type->AddIgnoreActors(List);
	}
}

void UIF_TargetCheckComponent::RemoveIgnoreActor(AActor* RemoveActor)
{
	if (Type)
	{
		Type->RemoveIgnoreActor(RemoveActor);
	}
}

void UIF_TargetCheckComponent::RemoveIgnoreActors(const TArray<AActor*> List)
{
	if (Type)
	{
		Type->RemoveIgnoreActors(List);
	}
}

void UIF_TargetCheckComponent::ClearIgnoreActors()
{
	if (Type)
	{
		Type->ClearIgnoreActors();
	}
}

AActor* UIF_TargetCheckComponent::GetCurrentTarget()
{
	return CurrentBestActor;
}

bool UIF_TargetCheckComponent::TryGetCurrentHitResult(FHitResult& HitResult)
{
	HitResult = CurrentBestHit;
	return HitResult.IsValidBlockingHit();
}


