// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_AI_Manager.h"
#include "InteractionFramework/Pool/IF_PoolItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionFramework/Core/IFSetting.h"


void UIF_AI_Manager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const auto Setting = UIFSetting::Get();
	DefaultPoolNum = Setting? Setting->DefaultPoolNum: 10;
	
	if (GetWorld() && GetWorld()->WorldType != EWorldType::Editor && Setting)
	{
		for (auto PD :Setting->AIPoolData)
		{
			AddPool(PD.Key, PD.Value);
		}
		
	}
}

void UIF_AI_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UIF_AI_Manager::RegisterAI(TScriptInterface<IIF_PoolItem> AI, FName Group)
{
	if (IsValid(AI.GetObject()))
	{
		IIF_PoolItem::Execute_OnRegisterAI(AI.GetObject());
		auto GroupValue = GroupData.Find(Group);
		if (GroupValue)
		{
			GroupValue->Emplace(AI);
		}
		else
		{
			GroupData.Emplace(Group,TArray<TScriptInterface<IIF_PoolItem>>{AI});
		}
	
		OnAI_Generate.Broadcast(AI, Group);
	}
}

void UIF_AI_Manager::RemoveAI(TScriptInterface<IIF_PoolItem> AI,  FName Group)
{
	if (IsValid(AI.GetObject()))
	{
		if(auto Pool = AI_Pool.Find(AI.GetObject()->GetClass()))
		{
			if (auto Idx = Pool->Find(AI))
			{
				(*Pool)[Idx] = nullptr;
			}
		}
		if (auto GroupValue = GroupData.Find(Group))
		{
			if (auto Idx = GroupValue->Find(AI))
			{
				(*GroupValue)[Idx] = nullptr;
			}
		}
		IIF_PoolItem::Execute_OnRemoveAI(AI.GetObject());
	}
	
}

TScriptInterface<IIF_PoolItem> UIF_AI_Manager::RetrieveAI(TSubclassOf<AActor> Class)
{

	auto Pool = AI_Pool.Find(Class);
	if (Pool)
	{
		for (auto AI: *Pool)
		{
			if (AI ==  nullptr)
			{
				return AI;
			}
		}
		const int32 OldNum = Pool->Num();
		AddPool(Class, OldNum);
		if (Pool->Num()>0 && Pool->IsValidIndex(OldNum))
		{
			return (*Pool)[OldNum];
		}
	}
	else
	{
		AddPool(Class, 10);
		
	}
	
	return  nullptr;
}

void UIF_AI_Manager::AddPool(TSubclassOf<AActor> Class, int32 Num)
{
	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnTransform.SetTranslation(FVector(0,0,5000));
	for (auto i = 0; i< Num; i ++)
	{
		auto Item = GetWorld()->SpawnActor<AActor>(Class, SpawnTransform, SpawnParameters);
		if (auto Character = Cast<ACharacter>(Item))
		{
			Character->GetCharacterMovement()->GravityScale = 0;
		}
		
		if (Item)
		{
			if(auto Value = AI_Pool.Find(Class))
			{
				Value->Emplace(Item);
			}
			else
			{
				AI_Pool.Emplace(Class,TArray<TScriptInterface<IIF_PoolItem>>{Item});
			}
			
		}
	}
}

