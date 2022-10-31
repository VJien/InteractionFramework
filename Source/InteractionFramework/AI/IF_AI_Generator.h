// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IF_AI_Manager.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "IF_AI_Generator.generated.h"

UCLASS()
class INTERACTIONFRAMEWORK_API AIF_AI_Generator : public ATargetPoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIF_AI_Generator();

protected:
	// Called when the game starts or when Generateed
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Run();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bAutoRun = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	float GenerateDelay = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	TArray<TSubclassOf<ACharacter>> AI_Class;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	FName Group = TEXT("Group_1");
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bMultiGenerate = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(MakeEditWidget = true, EditCondition="bMultiGenerate"))
	TArray<FTransform> MultiGeneratePoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="!bRandomGenerateNum && bMultiGenerate"))
	int32 GenerateNum = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="bMultiGenerate"))
	bool bRandomGenerateNum = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="bRandomGenerateNum && bMultiGenerate"))
	int32 GenerateNumMin = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="bRandomGenerateNum && bMultiGenerate"))
	int32 GenerateNumMax = 3;


	TWeakObjectPtr<UIF_AI_Manager> Manager = nullptr;

	UFUNCTION()
	void OnAI_ActorDestroyed(AActor* DestroyedActor);


	
};
