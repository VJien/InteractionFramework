// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ListenForAction.h"

#include "Kismet/GameplayStatics.h"

UTask_ListenForAction* UTask_ListenForAction::ListenForInputAction(UObject* Context, FName ActionName)
{
	UTask_ListenForAction* Obj = NewObject<UTask_ListenForAction>();
	Obj->PC =  UGameplayStatics::GetPlayerController(Context,0);
	Obj->Action = ActionName;

	if (!IsValid(Obj) || ActionName == NAME_None || !Obj->PC)
	{
		Obj->RemoveFromRoot();
		return nullptr;
	}
	Obj->Handle = Obj->PC->InputComponent->BindAction(ActionName, EInputEvent::IE_Pressed, Obj,&UTask_ListenForAction::KeyPressed);
	Obj->Handle = Obj->PC->InputComponent->BindAction(ActionName, EInputEvent::IE_Released, Obj,&UTask_ListenForAction::KeyReleased);

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
