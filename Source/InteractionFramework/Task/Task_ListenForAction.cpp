// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ListenForAction.h"

UTask_ListenForAction* UTask_ListenForAction::ListenForInputAction(APlayerController* Controller, FName ActionName)
{
	UTask_ListenForAction* ListenForAction = NewObject<UTask_ListenForAction>();
	ListenForAction->PC = Controller;
	ListenForAction->Action = ActionName;

	if (!IsValid(ListenForAction) || ActionName == NAME_None || !Controller)
	{
		ListenForAction->RemoveFromRoot();
		return nullptr;
	}
	ListenForAction->Handle = Controller->InputComponent->BindAction(ActionName, EInputEvent::IE_Pressed, ListenForAction,&UTask_ListenForAction::KeyPressed);
	ListenForAction->Handle = Controller->InputComponent->BindAction(ActionName, EInputEvent::IE_Released, ListenForAction,&UTask_ListenForAction::KeyReleased);

	return ListenForAction;
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
