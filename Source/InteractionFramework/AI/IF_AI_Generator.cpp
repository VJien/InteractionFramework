// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_AI_Generator.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionFramework/Pool/IF_PoolItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

PRAGMA_DISABLE_OPTIMIZATION
// Sets default values
AIF_AI_Generator::AIF_AI_Generator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AIF_AI_Generator::BeginPlay()
{
	Super::BeginPlay();
	Manager = GetWorld()->GetSubsystem<UIF_AI_Manager>(GetWorld());
	if (bAutoRun)
	{
		Run();
	}
}

// Called every frame
void AIF_AI_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIF_AI_Generator::OnAI_ActorDestroyed(AActor* DestroyedActor)
{
	if (DestroyedActor)
	{
		DestroyedActor->OnDestroyed.RemoveAll(this);
		if (Manager.IsValid())
		{
			if (UIF_PoolItem* AIInterface = Cast<UIF_PoolItem>(DestroyedActor))
			{
				Manager->DeleteAI(AIInterface, Group);
			}
		}
	}
}

void AIF_AI_Generator::Run_Implementation()
{
	if (AI_Class.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Generate Failed because of AI_Class.Num ==0"))
		return;
	}
	if (!GetWorld() || !(GetWorld()->WorldType == EWorldType::Game || GetWorld()->WorldType == EWorldType::PIE))
	{
		return;
	}
	if (Manager.IsValid())
	{
		if (bMultiGenerate)
		{
			const int32 Num = bRandomGenerateNum
				                  ? UKismetMathLibrary::RandomIntegerInRange(GenerateNumMin, GenerateNumMax)
				                  : GenerateNum;
			if (Num <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("AI Generate Failed because of Num <=0"))
			}
			if (GenerateDelay > 0)
			{
				FTimerHandle Handle;
				GetWorldTimerManager().SetTimer(Handle, [=]()
				{
					for (auto i = 0; i < Num; i++)
					{
						auto AIClass = AI_Class[UKismetMathLibrary::RandomIntegerInRange(0, AI_Class.Num() - 1)];
						auto AI = Manager->RetrieveAI(AIClass);
					if (IsValid(AI.GetObject()))
					{
						if (auto Actor = Cast<AActor>(AI.GetObject()))
						{
							Actor->OnDestroyed.AddDynamic(this, &AIF_AI_Generator::OnAI_ActorDestroyed);
							FTransform GeneratePoint = GetActorTransform();
							const int32 GenerateNum = MultiGeneratePoints.Num();
							if (GenerateNum > 0)
							{
								if (GenerateNum >= Num)
								{
									GeneratePoint = MultiGeneratePoints[Num];
								}
								else
								{
									GeneratePoint = MultiGeneratePoints[Num % GenerateNum];
								}
								
							}
							Actor->SetActorTransform(GeneratePoint);
						}
							Manager->RegisterAI(AI, Group);
						}
					}
				}, GenerateDelay, false);
			}
			else
			{
				for (auto i = 0; i < Num; i++)
				{
					auto AIClass = AI_Class[UKismetMathLibrary::RandomIntegerInRange(0, AI_Class.Num() - 1)];
					auto AI = Manager->RetrieveAI(AIClass);
					if (IsValid(AI.GetObject()))
					{
						if (auto Actor = Cast<AActor>(AI.GetObject()))
						{
							Actor->OnDestroyed.AddDynamic(this, &AIF_AI_Generator::OnAI_ActorDestroyed);
							FTransform GeneratePoint = GetActorTransform();
							const int32 GeneratePointsNum = MultiGeneratePoints.Num();
							if (GeneratePointsNum > 0)
							{
								if (GeneratePointsNum >= Num)
								{
									GeneratePoint = MultiGeneratePoints[Num];
								}
								else
								{
									GeneratePoint = MultiGeneratePoints[Num % GeneratePointsNum];
								}
								
							}
							Actor->SetActorTransform(GeneratePoint);
						}
						Manager->RegisterAI(AI, Group);
					}
				}
			}
		}
		else
		{
			if (GenerateDelay > 0)
			{
				FTimerHandle Handle;
				GetWorldTimerManager().SetTimer(Handle, [=]()
				{
					auto AIClass = AI_Class[UKismetMathLibrary::RandomIntegerInRange(0, AI_Class.Num() - 1)];
					auto AI = Manager->RetrieveAI(AIClass);
					if (IsValid(AI.GetObject()))
					{
						if (auto Actor = Cast<AActor>(AI.GetObject()))
						{
							Actor->OnDestroyed.AddDynamic(this, &AIF_AI_Generator::OnAI_ActorDestroyed);
							Actor->SetActorTransform(GetActorTransform());
						}
						Manager->RegisterAI(AI, Group);
				}
				}, GenerateDelay, false);
			}
			else
			{
				auto AIClass = AI_Class[UKismetMathLibrary::RandomIntegerInRange(0, AI_Class.Num() - 1)];
				auto AI = Manager->RetrieveAI(AIClass);
				if (IsValid(AI.GetObject()))
				{
					if (auto Actor = Cast<AActor>(AI.GetObject()))
					{
						Actor->OnDestroyed.AddDynamic(this, &AIF_AI_Generator::OnAI_ActorDestroyed);
						Actor->SetActorTransform(GetActorTransform());
					}
					Manager->RegisterAI(AI, Group);
				}
			}
		}
	}
}
PRAGMA_ENABLE_OPTIMIZATION