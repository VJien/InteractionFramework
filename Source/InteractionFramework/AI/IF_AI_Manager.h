// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "IF_AI_Manager.generated.h"


class IIF_PoolItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIF_AIEvent, TScriptInterface<IIF_PoolItem>, AI, FName, Group);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIF_AIEvent_NumberUpdate, TScriptInterface<IIF_PoolItem>, LastAI, int32, NewNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIF_AIEvent_GroupNumberUpdate, TScriptInterface<IIF_PoolItem>, LastAI, FName,Group,int32, NewNumber);

USTRUCT()
struct FPoolData
{
	GENERATED_BODY()
	FPoolData()
	{}
	FPoolData(TScriptInterface<IIF_PoolItem> InPoolItem, bool bInIsClean):PoolItem(InPoolItem), bIsClean(bInIsClean)
	{
	}
	UPROPERTY()
	TScriptInterface<IIF_PoolItem> PoolItem = nullptr;
	UPROPERTY()
	bool bIsClean = true;
};

UCLASS(DisplayName="AI Manager")
class INTERACTIONFRAMEWORK_API UIF_AI_Manager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return TStatId(); };

public:
	UFUNCTION(BlueprintCallable)
	void RegisterAI(TScriptInterface<IIF_PoolItem> AI, FName Group);
	UFUNCTION(BlueprintCallable)
	void ResetAI(TScriptInterface<IIF_PoolItem> AI,  FName Group);
	UFUNCTION(BlueprintCallable)
	void DeleteAI(TScriptInterface<IIF_PoolItem> AI,  FName Group);
	UFUNCTION(BlueprintCallable)
	void InitPool();

	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetActiveAI_Number();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetActiveAI_NumberByGroup(FName Group);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetActiveAI_NumberByClass(TSubclassOf<ACharacter> Class);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetActiveAI(TArray<TScriptInterface<IIF_PoolItem>>& AI);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetActiveAI_ByGroup(FName Group,TArray<TScriptInterface<IIF_PoolItem>>& AI);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetActiveAI_ByClass(TSubclassOf<ACharacter> Class, TArray<TScriptInterface<IIF_PoolItem>>& AI);
	
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TScriptInterface<IIF_PoolItem> RetrieveAI(TSubclassOf<ACharacter> Class);
	

	TArray<FPoolData>* AddPool(TSubclassOf<ACharacter> Class, int32 Num = 10);

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FIF_AIEvent OnAI_Generate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FIF_AIEvent OnAI_Reset;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FIF_AIEvent_NumberUpdate OnAI_NumberAdd;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FIF_AIEvent_NumberUpdate OnAI_NumberRemove;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FIF_AIEvent_GroupNumberUpdate OnAI_GroupNumberAdd;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FIF_AIEvent_GroupNumberUpdate OnAI_GroupNumberRemove;

	

	int32 DefaultPoolNum = 10;
	TMap<TSubclassOf<ACharacter>, TArray<FPoolData>> Pool;
	TMap<FName, TArray<FPoolData>> GroupData;

	TArray<TScriptInterface<IIF_PoolItem>> ActiveAI;
};
