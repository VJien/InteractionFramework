// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ListenForAxis.h"

UTask_ListenForAxis* UTask_ListenForAxis::ListenForAxis(APlayerController* Controller, FName AxisName)
{
	UTask_ListenForAxis* Obj = NewObject<UTask_ListenForAxis>();
	Obj->PC = Controller;
	Obj->Axis = AxisName;

	if (!IsValid(Obj) || AxisName == NAME_None || !Controller)
	{
		Obj->RemoveFromRoot();
		return nullptr;
	}
	Obj->Handle = Controller->InputComponent->BindAxis(AxisName,  Obj,&UTask_ListenForAxis::AxisUpdate);

	return Obj;
}

void UTask_ListenForAxis::EndTask()
{
	SetReadyToDestroy();
	MarkPendingKill();
}

void UTask_ListenForAxis::AxisUpdate(float Value)
{
	OnAxisUpdate.Broadcast(Value);
}

