// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_VRMovement.h"

#include "GameFramework/Character.h"

PRAGMA_DISABLE_OPTIMIZATION
// Sets default values for this component's properties
UIF_VRMovement::UIF_VRMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIF_VRMovement::BeginPlay()
{
	Super::BeginPlay();
	InitInput();
	if (auto Pawn = Cast<APawn>(GetOwner()))
	{
		MovementComponent = Pawn->GetMovementComponent();
	}
	// ...
	
}


// Called every frame
void UIF_VRMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UIF_VRMovement::InitInput()
{
	if (MovementAction.Num() == 0)
	{
		return;
	}
	auto PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (!PC)
	{
		return;
	}
	mInputComponent = PC->InputComponent;
	if (mInputComponent.IsValid())
	{
		for (auto MA : MovementAction)
		{
			if (auto AC = Cast<UIF_InputTypeConfig_Movement_Action>(MA))
			{
				BindAction(mInputComponent.Get(), AC);
				
			}
			else if (auto AX = Cast<UIF_InputTypeConfig_Movement_Axis>(MA))
			{
				BindAxis(mInputComponent.Get(), AX);
			}
		}
	}
	
}

void UIF_VRMovement::BindAction(UInputComponent* InputComponent, UIF_InputTypeConfig_Movement_Action* AC)
{

	switch (AC->MoveType)
	{
	case EVRMovementType::Walk:
		{
			BIND_ACTION(InputComponent, AC->Action,  AC->Event, Walk)
		}
	case EVRMovementType::PreTeleport:
		{
			BIND_ACTION(InputComponent, AC->Action, AC->Event, PreTeleport)
		}
	case EVRMovementType::Teleport:
		{
			BIND_ACTION(InputComponent, AC->Action, AC->Event, Teleport)
		}
	case EVRMovementType::Jump:
		{
			BIND_ACTION(InputComponent, AC->Action, AC->Event, Jump)
		}
	case EVRMovementType::PreJumpCurve:
		{
			BIND_ACTION(InputComponent, AC->Action, AC->Event, PreJumpCurve)
		}
	case EVRMovementType::JumpCurve:
		{
			BIND_ACTION(InputComponent, AC->Action, AC->Event, JumpCurve)
		}
	case EVRMovementType::PreSprint:
		{
			BIND_ACTION(InputComponent, AC->Action, AC->Event, PreSprint)
		}
	case EVRMovementType::Sprint:
		{
			BIND_ACTION(InputComponent, AC->Action, AC->Event, Sprint)
		}
	case EVRMovementType::Turn:
		{
			BIND_ACTION(InputComponent, AC->Action, AC->Event, Turn)
		}
		default:
			break;
	}
}

void UIF_VRMovement::BindAxis(UInputComponent* InputComponent,   UIF_InputTypeConfig_Movement_Axis* AX)
{
	switch (AX->MoveType)
	{
	case EVRMovementType::Walk:
		{
			BIND_AXIS(InputComponent, AX->Axis, Walk)
		}
	case EVRMovementType::PreTeleport:
		{
			BIND_AXIS(InputComponent, AX->Axis, PreTeleport)
		}
	case EVRMovementType::Teleport:
		{
			BIND_AXIS(InputComponent, AX->Axis, Teleport)
		}
	case EVRMovementType::Jump:
		{
			BIND_AXIS(InputComponent, AX->Axis, Jump)
		}
	case EVRMovementType::PreJumpCurve:
		{
			BIND_AXIS(InputComponent, AX->Axis, PreJumpCurve)
		}
	case EVRMovementType::JumpCurve:
		{
			BIND_AXIS(InputComponent, AX->Axis, JumpCurve)
		}
	case EVRMovementType::PreSprint:
		{
			BIND_AXIS(InputComponent, AX->Axis, PreSprint)
		}
	case EVRMovementType::Sprint:
		{
			BIND_AXIS(InputComponent, AX->Axis, Sprint)
		}
	case EVRMovementType::Turn:
		{
			BIND_AXIS(InputComponent, AX->Axis, Turn)
		}
	default:
		break;
	}
}

void UIF_VRMovement::SetupWalkData(FIF_WalkData Data)
{
	WalkData = Data;
}

bool UIF_VRMovement::CheckAxis(EVRMovementType Type,float Axis)
{
	for (auto P: MovementAction)
	{
		if (P->MoveType != Type)
		{
			break;
		}
		if (auto Ax = Cast<UIF_InputTypeConfig_Movement_Axis>(P))
		{
			float NewValue = Ax->bAbsValue? FMath::Abs(Axis): Axis;
			switch (Ax->Function)
			{
			case EMoveAxisFunction::Equal:
				{
					return NewValue == Ax->Value;
				}
			case EMoveAxisFunction::Less:
				{
					return NewValue < Ax->Value;
				}
			case EMoveAxisFunction::LessOrEqual:
				{
					return NewValue <= Ax->Value;
				}
			case EMoveAxisFunction::Greater:
				{
					return NewValue > Ax->Value;
				}
			case EMoveAxisFunction::GreaterOrEqual:
				{
					return NewValue >= Ax->Value;
				}
			default:
				break;
				
			}
		}
		
	}
	return false;
}




void UIF_VRMovement::DoPreTeleport_Implementation(float Value)
{
}
void UIF_VRMovement::DoTeleport_Implementation(float Value)
{
}
void UIF_VRMovement::DoTurn_Implementation(float Value)
{
}
void UIF_VRMovement::DoPreSprint_Implementation(float Value)
{
}
void UIF_VRMovement::DoSprint_Implementation(float Value)
{
}
void UIF_VRMovement::DoJump_Implementation(float Value)
{
}
void UIF_VRMovement::DoPreJumpCurve_Implementation(float Value)
{
}
void UIF_VRMovement::DoWalk_Implementation(float Value)
{
}
void UIF_VRMovement::DoJumpCurve_Implementation(float Value)
{
}

PRAGMA_ENABLE_OPTIMIZATION