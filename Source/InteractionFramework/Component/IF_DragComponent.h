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
	EIF_DragType_Source SourceType = EIF_DragType_Source::Linear_X;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="SourceType == EIF_DragType_Source::Linear_X || SourceType == EIF_DragType_Source::Linear_Y || SourceType == EIF_DragType_Source::Linear_Z"))
	bool bSourceInTargetSpace = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="SourceType != EIF_DragType_Source::Linear && SourceType != EIF_DragType_Source::Rotation && SourceType != EIF_DragType_Source::Angle"))
	EIF_DragType_Target TargetType = EIF_DragType_Target::Linear_X;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="TargetType == EIF_DragType_Target::Linear_X || TargetType == EIF_DragType_Target::Linear_Y || TargetType == EIF_DragType_Target::Linear_Z"))
	bool bTargetTypeIsWorldSpace = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	float Scale = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config)
	bool bClamp = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="bClamp"))
	float Max = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Config, meta=(EditCondition="bClamp"))
	float Min = 0;

	UPROPERTY()
	USceneComponent* DragSourceComponent = nullptr;

	bool bIsDraging = false;

	FTransform SourceComponentStartTM;
	FTransform DragComponentStartTM;
	FTransform RelativeTM_Angle;
	FTransform RelativeTM_Roll;
	FTransform RelativeTM_Pitch;
	FTransform RelativeTM_Yaw;
	FTransform RelativeTM;
};
