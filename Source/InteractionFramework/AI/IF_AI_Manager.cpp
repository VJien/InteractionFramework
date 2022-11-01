// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_AI_Manager.h"
#include "InteractionFramework/Pool/IF_PoolItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionFramework/Core/IFSetting.h"
#include "Kismet/KismetSystemLibrary.h"

PRAGMA_DISABLE_OPTIMIZATION

void UIF_AI_Manager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UIF_AI_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UIF_AI_Manager::RegisterAI(TScriptInterface<IIF_PoolItem> AI, FName Group)
{
	if (IsValid(AI.GetObject()))
	{
		IIF_PoolItem::Execute_OnRegister(AI.GetObject());
		auto GroupValue = GroupData.Find(Group);
		if (GroupValue)
		{
			GroupValue->Emplace(AI,false);
		}
		else
		{
			GroupData.Emplace(Group,TArray<FPoolData>{FPoolData(AI,false)});
		}
		OnAI_Generate.Broadcast(AI, Group);
		if (auto Actor = Cast<AActor>(AI.GetObject()))
		{
			Actor->SetActorTickEnabled(true);
			
		}
		if (auto Character = Cast<ACharacter>(AI.GetObject()))
		{
			Character->GetCharacterMovement()->GravityScale = 1;
		}
		
	}
}

void UIF_AI_Manager::ResetAI(TScriptInterface<IIF_PoolItem> AI,  FName Group)
{
	if (IsValid(AI.GetObject()))
	{
		if(auto PoolData = Pool.Find(AI.GetObject()->GetClass()))
		{
			for (auto& P: *PoolData)
			{
				if (P.PoolItem == AI)
				{
					P.bIsClean = true;
				}
			}
		}
		if (auto GroupValue = GroupData.Find(Group))
		{
			for (auto& P: *GroupValue)
			{
				if (P.PoolItem == AI)
				{
					P.bIsClean = true;
				}
			}
		}
		IIF_PoolItem::Execute_OnReset(AI.GetObject());
		OnAI_Reset.Broadcast(AI,Group);
	}
	
}

void UIF_AI_Manager::DeleteAI(TScriptInterface<IIF_PoolItem> AI, FName Group)
{
	if (IsValid(AI.GetObject()))
	{
		if(auto PoolData = Pool.Find(AI.GetObject()->GetClass()))
		{
			for (int32 i=0; i< PoolData->Num(); ++i)
			{
				if ((*PoolData)[i].PoolItem == AI)
				{
					(*PoolData).RemoveAt(i);
					break;
				}
			}
		}
		if (auto GroupValue = GroupData.Find(Group))
		{
			for (int32 i=0; i< GroupValue->Num(); ++i)
			{
				if ((*GroupValue)[i].PoolItem == AI)
				{
					(*GroupValue).RemoveAt(i);
					break;
				}
			}
		}
	}
}

void UIF_AI_Manager::InitPool()
{
	const auto Setting = UIFSetting::Get();
	DefaultPoolNum = Setting? Setting->DefaultPoolNum: 10;
	
	if (GetWorld() && (GetWorld()->WorldType == EWorldType::Game ||GetWorld()->WorldType == EWorldType::PIE) && Setting)
	{
		for (auto P :Setting->AIPoolData)
		{
			auto Class = LoadClass<ACharacter>(NULL,*P.Key.ToString(),NULL);
			if (Class)
			{
				AddPool(Class, P.Value);
			}
		}
		
	}
}

TScriptInterface<IIF_PoolItem> UIF_AI_Manager::RetrieveAI(TSubclassOf<ACharacter> Class)
{

	auto PoolData = Pool.Find(Class);
	if (PoolData)
	{
		for (auto P: *PoolData)
		{
			if (P.bIsClean == true)
			{
				return P.PoolItem;
			}
		}
		const int32 OldNum = PoolData->Num();
		AddPool(Class, OldNum);
		if (PoolData->Num()>0 && PoolData->Num() >= OldNum)
		{
			for (int32 i = OldNum; i< PoolData->Num(); ++i)
			{
				if ((*PoolData)[i].bIsClean == true)
				{
					return (*PoolData)[i].PoolItem;
				}
			}
		}
	}
	else
	{
		auto NewPoolData = AddPool(Class, DefaultPoolNum);
		if (NewPoolData)
		{
			for (int32 i = 0; i< NewPoolData->Num(); ++i)
			{
				if ((*NewPoolData)[i].bIsClean == true)
				{
					return (*NewPoolData)[i].PoolItem;
				}
			}
		}
		
	}
	return  nullptr;
}

TArray<FPoolData>* UIF_AI_Manager::AddPool(TSubclassOf<ACharacter> Class, int32 Num)
{
	if (!UKismetSystemLibrary::DoesImplementInterface(Class->GetDefaultObject(),UIF_PoolItem::StaticClass()))
	{
		return nullptr;
	}
	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnTransform.SetTranslation(FVector(0,0,5000));
	for (auto i = 0; i< Num; i ++)
	{
		auto Item = GetWorld()->SpawnActor<ACharacter>(Class, SpawnTransform, SpawnParameters);
		if (Item)
		{
			Item->GetCharacterMovement()->GravityScale = 0;
			Item->SetActorTickEnabled(false);
			auto PoolData = Pool.Find(Class);
			if(PoolData)
			{
				PoolData->Emplace(Item,true);
			}
			else
			{
				Pool.Emplace(Class,TArray<FPoolData>{FPoolData(Item,true)});
			}
		}
	}
	return Pool.Find(Class);
}

PRAGMA_ENABLE_OPTIMIZATION