﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/IF_ActorComponent.h"
#include "Base/IF_SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IF_TargetCheckComponent.generated.h"
UENUM(BlueprintType)
enum class EIF_TargetCheckMultiResultRule : uint8
{
	Default,
	Near,
	Far
};

UCLASS(BlueprintType, Blueprintable, Abstract,EditInlineNew)
class UIF_TargetCheckConfig : public UObject
{
	GENERATED_BODY()
protected:
	bool DoesHitResultMatched(const FHitResult& HitResult);
	bool DoesActorMatched(AActor* Actor);
public:
	UFUNCTION(BlueprintCallable)
	void AddIgnoreActors(const TArray<AActor*> List);
	UFUNCTION(BlueprintCallable)
	void RemoveIgnoreActor(AActor* RemoveActor);
	UFUNCTION(BlueprintCallable)
	void RemoveIgnoreActors(const TArray<AActor*> List);
	UFUNCTION(BlueprintCallable)
	void ClearIgnoreActors();
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetActor(USceneComponent* SourceCompennt);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FHitResult GetHitResult(USceneComponent* SourceCompennt);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Common)
	bool bIgnoreOwner = true;
	//开启类型过滤, 如果下面没有任何过滤参数, 建议关闭此值, 节省开销
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Common")
	bool bEnableFilter = true;
	//检测类的类型, 空以为所有
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Common|Filter", meta=(EditCondition="bEnableFilter"))
	TSubclassOf<AActor> ClassOnlyCheck= nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Common|Filter", meta=(EditCondition="bEnableFilter"))
	TArray<TSubclassOf<AActor>> ClassesIgnore;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Common|Filter", meta=(EditCondition="bEnableFilter"))
	TArray<FName> ActorWithTags;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Common|Filter", meta=(EditCondition="bEnableFilter"))
	TArray<FName> ActorWithoutTags;
	
	UPROPERTY()
	TArray<AActor*> IgnoreActors;
};

//*****************Trace**********************
UCLASS(BlueprintType, Blueprintable, Abstract,EditInlineNew)
class UIF_TargetCheckConfig_TraceBase : public UIF_TargetCheckConfig
{
	GENERATED_BODY()

public:
	virtual FHitResult GetBestMultiHitResult(USceneComponent* SourceCompennt, const TArray<FHitResult> HitResults);
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Trace)
	float Distance = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Trace)
	bool bMultiTrace = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Trace, meta=(EditCondition="bMultiTrace"))
	EIF_TargetCheckMultiResultRule MultiCheckRule = EIF_TargetCheckMultiResultRule::Default;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Trace)
	bool bUseChannel = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Trace, meta=(EditCondition="bUseChannel"))
	TEnumAsByte<ETraceTypeQuery> Channel = ETraceTypeQuery::TraceTypeQuery1;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Trace, meta=(EditCondition="!bUseChannel"))
	TArray<TEnumAsByte<EObjectTypeQuery>> Collision;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Trace)
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;
	
};
UCLASS(BlueprintType, DisplayName=SphereTrace)
class UIF_TargetCheckConfig_Sphere : public UIF_TargetCheckConfig_TraceBase
{
	GENERATED_BODY()
	
public:
	virtual AActor* GetActor_Implementation(USceneComponent* SourceCompennt) override;
	virtual FHitResult GetHitResult_Implementation(USceneComponent* SourceCompennt) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=SphereTrace)
	float Radius = 10.f;
};
UCLASS(BlueprintType, DisplayName=BoxTrace)
class UIF_TargetCheckConfig_Box : public UIF_TargetCheckConfig_TraceBase
{
	GENERATED_BODY()
public:
	virtual AActor* GetActor_Implementation(USceneComponent* SourceCompennt) override;
	virtual FHitResult GetHitResult_Implementation(USceneComponent* SourceCompennt) override;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=BoxTrace)
	FVector Extend = FVector(10.0f);
};
//**************************Overlap***************************
//该类型只有获取Actor的方式
UCLASS(BlueprintType, Abstract)
class UIF_TargetCheckConfig_Overlap : public UIF_TargetCheckConfig
{
	GENERATED_BODY()
protected:
	virtual UPrimitiveComponent* CreateOverlapComponent(USceneComponent* SourceCompennt);
	virtual AActor* GetBestActor(USceneComponent* SourceCompennt, const TArray<AActor*>& Actors);
public:
	virtual AActor* GetActor_Implementation(USceneComponent* SourceCompennt) override;
	
	UPrimitiveComponent* OverlapComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Overlap)
	TEnumAsByte<ECollisionChannel> CollisionType = ECC_WorldDynamic;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Overlap, meta=(EditCondition="bMultiTrace"))
	EIF_TargetCheckMultiResultRule MultiCheckRule = EIF_TargetCheckMultiResultRule::Default;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Overlap)
	bool bDrawDebug = false;

};
//该类型只有获取Actor的方式
UCLASS(BlueprintType, DisplayName=Sphere)
class UIF_TargetCheckConfig_SphereOverlap : public UIF_TargetCheckConfig_Overlap
{
	GENERATED_BODY()
protected:
	virtual UPrimitiveComponent* CreateOverlapComponent(USceneComponent* SourceCompennt) override;
public:
	virtual AActor* GetActor_Implementation(USceneComponent* SourceCompennt) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Sphere)
	float Radius = 10.f;
};
//该类型只有获取Actor的方式
UCLASS(BlueprintType, DisplayName=Box)
class UIF_TargetCheckConfig_BoxOverlap : public UIF_TargetCheckConfig_Overlap
{
	GENERATED_BODY()
protected:
	virtual UPrimitiveComponent* CreateOverlapComponent(USceneComponent* SourceCompennt) override;
public:
	virtual AActor* GetActor_Implementation(USceneComponent* SourceCompennt) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Box)
	FVector Extent = FVector(10);
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHandCheckActorEvent, AActor*, NewActor, AActor*, OldActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHandCheckHitEvent, FHitResult, NewHit,FHitResult, OldHit);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONFRAMEWORK_API UIF_TargetCheckComponent : public UIF_SceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_TargetCheckComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
	void AddIgnoreActors(const TArray<AActor*> List);
	UFUNCTION(BlueprintCallable)
	void RemoveIgnoreActor(AActor* RemoveActor);
	UFUNCTION(BlueprintCallable)
	void RemoveIgnoreActors(const TArray<AActor*> List);
	UFUNCTION(BlueprintCallable)
	void ClearIgnoreActors();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetCurrentTarget();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool TryGetCurrentHitResult(FHitResult& HitResult);
	
protected:
	//检测间隔, 0:高频Tick
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	float CheckInterval = 0.1;
	//保持记住最后一个目标
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bRememberLastTarget = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, Instanced)
	UIF_TargetCheckConfig* Type;
	
	

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHandCheckActorEvent OnHitActorUpdated;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHandCheckHitEvent OnHandCheckHitResultUpdated;
	
	UPROPERTY()
	AActor* CurrentBestActor = nullptr;
	UPROPERTY()
	FHitResult CurrentBestHit;




	float CurrTime= 0;

	
};
