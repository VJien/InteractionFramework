// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/IF_SceneComponent.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "IF_DragComponent.generated.h"


UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent))
class INTERACTIONFRAMEWORK_API UIF_DragComponent : public UIF_SceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_DragComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateDrag(float Delta);

	void SetDragOffset(float Offset);
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool StartDrag(USceneComponent* SourceComponent);
	UFUNCTION(BlueprintCallable)
	void StopDrag(USceneComponent* SourceComponent);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bDebug = false;
	//哪只手操作
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_VRHandType HandType = EIF_VRHandType::None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_DragType SourceType = EIF_DragType::Linear_X;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bSourceInTargetSpace = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	EIF_DragType TargetType = EIF_DragType::Linear_X;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bTargetTypeIsWorldSpace = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	float Scale = 1;


	UPROPERTY()
	USceneComponent* DragSourceComponent = nullptr;

	bool bIsDraging = false;

	FTransform SourceComponentTransform;
	FTransform DragComponentTransform;
};
