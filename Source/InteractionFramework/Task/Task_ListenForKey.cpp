// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ListenForKey.h"


UTask_ListenForKey* UTask_ListenForKey::ListenForKey(APlayerController* Controller, FKey Key)
{
	UTask_ListenForKey* Obj = NewObject<UTask_ListenForKey>();
	Obj->PC = Controller;
	Obj->LocalKey = Key;

	if (!IsValid(Obj) || !Key.IsValid() || !Controller)
	{
		Obj->RemoveFromRoot();
		return nullptr;
	}
	Obj->Handle = Controller->InputComponent->BindKey(Key, EInputEvent::IE_Pressed, Obj,&UTask_ListenForKey::KeyPressed);
	Obj->Handle = Controller->InputComponent->BindKey(Key, EInputEvent::IE_Released, Obj,&UTask_ListenForKey::KeyReleased);
	Obj->Handle = Controller->InputComponent->BindKey(Key, EInputEvent::IE_DoubleClick, Obj,&UTask_ListenForKey::KeyDoubleClicked);
	return Obj;
}

void UTask_ListenForKey::EndTask()
{
	SetReadyToDestroy();
	MarkPendingKill();
}

void UTask_ListenForKey::KeyPressed()
{
	OnPressed.Broadcast();
}

void UTask_ListenForKey::KeyReleased()
{
	OnReleased.Broadcast();
}

void UTask_ListenForKey::KeyDoubleClicked()
{
	OnDoubleClicked.Broadcast();
}
