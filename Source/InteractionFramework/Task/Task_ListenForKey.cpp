// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ListenForKey.h"

#include "Kismet/GameplayStatics.h"


UTask_ListenForKey* UTask_ListenForKey::ListenForKey(UObject* Context, FKey Key)
{
	UTask_ListenForKey* Obj = NewObject<UTask_ListenForKey>();
	Obj->PC =  UGameplayStatics::GetPlayerController(Context,0);
	
	Obj->LocalKey = Key;

	if (!IsValid(Obj) || !Key.IsValid() || !Obj->PC)
	{
		Obj->RemoveFromRoot();
		return nullptr;
	}
	Obj->Handle = Obj->PC->InputComponent->BindKey(Key, EInputEvent::IE_Pressed, Obj,&UTask_ListenForKey::KeyPressed);
	Obj->Handle = Obj->PC->InputComponent->BindKey(Key, EInputEvent::IE_Released, Obj,&UTask_ListenForKey::KeyReleased);
	Obj->Handle = Obj->PC->InputComponent->BindKey(Key, EInputEvent::IE_DoubleClick, Obj,&UTask_ListenForKey::KeyDoubleClicked);
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
