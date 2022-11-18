// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UObject/Object.h"
#include "Task_ListenForAxis.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIF_OnAxisvent, float, value);


UCLASS()
class INTERACTIONFRAMEWORK_API UTask_ListenForAxis : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FIF_OnAxisvent OnAxisUpdate;


	// 监听 输入Action
	UFUNCTION(BlueprintCallable, Category = "IF|Task", meta = (HidePin = "Context", DefaultToSelf = "Context", BlueprintInternalUseOnly = "TRUE", AdvancedDisplay="2"))
		static UTask_ListenForAxis* ListenForAxis(UObject* Context, FName AxisName, bool bTriggerWhenPaused = true, bool bConsumeInput = true);



	UFUNCTION(BlueprintCallable)
		void EndTask();

protected:
	UPROPERTY()
	APlayerController* PC = nullptr;

	FName Axis;


	void AxisUpdate(float Value);
};
