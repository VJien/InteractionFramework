// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_ActionComponent.h"


// Sets default values for this component's properties
UIF_ActionComponent::UIF_ActionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	
	ActionFuncs.Emplace(&UIF_ActionComponent::Action0);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action1);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action2);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action3);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action4);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action5);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action6);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action7);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action8);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action9);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action10);
	ActionFuncs.Emplace(&UIF_ActionComponent::Action11);

	AxisFuncs.Emplace(&UIF_ActionComponent::Axis0);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis1);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis2);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis3);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis4);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis5);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis6);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis7);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis8);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis9);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis10);
	AxisFuncs.Emplace(&UIF_ActionComponent::Axis11);
	ActionName.SetNum(MaxEventNum);
	ActionEvent.SetNum(MaxEventNum);
	AxisName.SetNum(MaxEventNum);
	AxisEvent.SetNum(MaxEventNum);
	ActionConfigObj.AddDefaulted(MaxEventNum);
	AxisConfigObj.AddDefaulted(MaxEventNum);
}


// Called when the game starts
void UIF_ActionComponent::BeginPlay()
{
	Super::BeginPlay();
	if (auto Pawn =  Cast<APawn>(GetOwner()))
	{
		InputComponent = Pawn->InputComponent;
	}
	InitInputMap();
	
	
	
	// ...

}

void UIF_ActionComponent::InitInputMap()
{
	if (bHasInit)
	{
		return;
	}
	bHasInit = true;
	if (!InputComponent.IsValid())
	{
		return;
	}
	for (auto P : InputMap)
	{
		if (auto AC = Cast<UIF_InputTypeConfig_CustomEvent_Action>(P))
		{
			if (CurrBindIndex_Action >= MaxEventNum)
			{
				break;
			}
			ActionName[CurrBindIndex_Action] = (AC->Action);
			ActionEvent[CurrBindIndex_Action] =( AC->EventConfig);
			ActionConfigObj[CurrBindIndex_Action] = AC;
			InputComponent->BindAction(AC->Action, AC->KeyEvent, this, ActionFuncs[CurrBindIndex_Action]);
			CurrBindIndex_Action++;
				
		}
		else if (auto AX = Cast<UIF_InputTypeConfig_CustomEvent_Axis>(P))
		{
			if (CurrBindIndex_Axis >= MaxEventNum)
			{
				break;
			}
			AxisName[CurrBindIndex_Axis] =(AX->Axis);
			AxisEvent[CurrBindIndex_Axis]=(AX->EventConfig);
			AxisConfigObj[CurrBindIndex_Axis]=AX;
			InputComponent->BindAxis(AX->Axis,  this, AxisFuncs[CurrBindIndex_Axis]);
			CurrBindIndex_Axis++;
		}
	}
}


// Called every frame
void UIF_ActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UIF_ActionComponent::CheckAxisValue_Implementation(float Value, UIF_InputTypeConfig_CustomEvent_Axis* AxisConfig)
{
	if (!AxisConfig)
	{
		return false;
	}
	
	float NewValue = AxisConfig->bAbsValue? FMath::Abs(Value) : Value;
	switch (AxisConfig->Function)
	{
	case EMoveAxisFunction::Greater:
		{
			return NewValue > AxisConfig->Value;
		}
	case EMoveAxisFunction::GreaterOrEqual:
		{
			return NewValue >= AxisConfig->Value;
		}
	case EMoveAxisFunction::Equal:
		{
			return NewValue == AxisConfig->Value;
		}
	case EMoveAxisFunction::LessOrEqual:
		{
			return NewValue <= AxisConfig->Value;
		}
	case EMoveAxisFunction::Less:
		{
			return NewValue < AxisConfig->Value;
		}
		default:
			break;
		
		
	}
	return false;
}

void UIF_ActionComponent::ExcuteAxisEvent(UIF_EventConfig*  Event, float Value)
{
	ReceiveAxisEvent(Event, Value);
	OnAxisEventActivated.Broadcast(Event,Value);
}


void UIF_ActionComponent::ExcuteActionEvent(UIF_EventConfig* Event)
{
	ReceiveActionEvent(Event);
	OnActionEventActivated.Broadcast(Event);
}

