// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "IF_AI_Manager.generated.h"


class IIF_PoolItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIF_AIEvent, TScriptInterface<IIF_PoolItem>, AI, FName, Group);



UCLASS()
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
	void RemoveAI(TScriptInterface<IIF_PoolItem> AI,  FName Group);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TScriptInterface<IIF_PoolItem> RetrieveAI(TSubclassOf<AActor> Class);
	

	void AddPool(TSubclassOf<AActor> Class, int32 Num = 10);

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FIF_AIEvent OnAI_Generate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FIF_AIEvent OnAI_Removed;
	
	
	TMap<FName, TArray<TScriptInterface<IIF_PoolItem>>> GroupData;


	int32 DefaultPoolNum = 10;
	TMap<TSubclassOf<AActor>, TArray<TScriptInterface<IIF_PoolItem>>> AI_Pool;
};
