// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UObject/Object.h"
#include "Task_ListenForAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIF_OnActionEvent);


UCLASS()
class INTERACTIONFRAMEWORK_API UTask_ListenForAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FIF_OnActionEvent OnKeyPressed;
	UPROPERTY(BlueprintAssignable)
	FIF_OnActionEvent OnKeyReleased;

	// 监听 输入Action
	UFUNCTION(BlueprintCallable, Category = "IF|Task", meta = (HidePin = "Context", DefaultToSelf = "Context", BlueprintInternalUseOnly = "TRUE"))
		static UTask_ListenForAction* ListenForInputAction(UObject* Context, FName ActionName);



	UFUNCTION(BlueprintCallable)
		void EndTask();

protected:
	UPROPERTY()
	APlayerController* PC = nullptr;

	FName Action;
	FInputActionBinding Handle; 

	void KeyPressed();
	void KeyReleased();
};
