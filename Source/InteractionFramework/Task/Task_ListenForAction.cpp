// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ListenForAction.h"

#include "Kismet/GameplayStatics.h"

UTask_ListenForAction* UTask_ListenForAction::ListenForInputAction(UObject* Context, FName ActionName, bool bTriggerWhenPaused, bool bConsumeInput)
{
	UTask_ListenForAction* Obj = NewObject<UTask_ListenForAction>();
	Obj->PC =  UGameplayStatics::GetPlayerController(Context,0);
	Obj->Action = ActionName;

	if (!IsValid(Obj) || ActionName == NAME_None || !Obj->PC)
	{
		Obj->RemoveFromRoot();
		return nullptr;
	}
	auto& BindPress = Obj->PC->InputComponent->BindAction(ActionName, EInputEvent::IE_Pressed, Obj,&UTask_ListenForAction::KeyPressed);
	BindPress.bExecuteWhenPaused = bTriggerWhenPaused;
	BindPress.bConsumeInput = bConsumeInput;
	auto& DoubleClickBind = Obj->PC->InputComponent->BindAction(ActionName, EInputEvent::IE_Released, Obj,&UTask_ListenForAction::KeyReleased);
	DoubleClickBind.bExecuteWhenPaused = bTriggerWhenPaused;
	DoubleClickBind.bConsumeInput = bConsumeInput;
	return Obj;
}


void UTask_ListenForAction::EndTask()
{
	SetReadyToDestroy();
	MarkPendingKill();
}

void UTask_ListenForAction::KeyPressed()
{
	OnKeyPressed.Broadcast();
}

void UTask_ListenForAction::KeyReleased()
{
	OnKeyReleased.Broadcast();
}
