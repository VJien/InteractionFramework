﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ListenForAxis.h"

#include "Kismet/GameplayStatics.h"

UTask_ListenForAxis* UTask_ListenForAxis::ListenForAxis(UObject* Context, FName AxisName)
{
	UTask_ListenForAxis* Obj = NewObject<UTask_ListenForAxis>();
	Obj->PC =  UGameplayStatics::GetPlayerController(Context,0);
	Obj->Axis = AxisName;

	if (!IsValid(Obj) || AxisName == NAME_None || !Obj->PC)
	{
		Obj->RemoveFromRoot();
		return nullptr;
	}
	Obj->Handle = Obj->PC->InputComponent->BindAxis(AxisName,  Obj,&UTask_ListenForAxis::AxisUpdate);

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

