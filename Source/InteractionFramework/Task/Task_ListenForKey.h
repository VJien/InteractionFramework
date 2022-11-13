// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UObject/Object.h"
#include "Task_ListenForKey.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIF_OnKeyEvent);


UCLASS()
class INTERACTIONFRAMEWORK_API UTask_ListenForKey : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FIF_OnKeyEvent OnPressed;
	UPROPERTY(BlueprintAssignable)
	FIF_OnKeyEvent OnReleased;
	UPROPERTY(BlueprintAssignable)
	FIF_OnKeyEvent OnDoubleClicked;


	// 监听 输入Action
	UFUNCTION(BlueprintCallable, Category = "IF|Task", meta = (HidePin = "Context", DefaultToSelf = "Context", BlueprintInternalUseOnly = "TRUE"))
		static UTask_ListenForKey* ListenForKey(UObject* Context, FKey Key);



	UFUNCTION(BlueprintCallable)
		void EndTask();

protected:
	UPROPERTY()
	APlayerController* PC = nullptr;

	FKey LocalKey;
	FInputKeyBinding Handle; 

	void KeyPressed();
	void KeyReleased();
	void KeyDoubleClicked();
};
