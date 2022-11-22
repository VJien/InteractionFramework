// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IFTypes.generated.h"

UENUM(BlueprintType)
enum class EIF_MoveAxisFunction: uint8
{
	Equal,
	Greater,
	GreaterOrEqual,
	Less,
	LessOrEqual,
};

UENUM(BlueprintType)
enum class EIF_VRMovementType: uint8
{
	None,
	PreTeleport,
	Teleport,
	Walk,
	Turn,
	PreSprint,
	Sprint,
	Jump,
	PreJumpCurve,
	JumpCurve
};
UENUM(BlueprintType)
enum class EIF_VRPlayerAction: uint8
{
	None,
	Move,
	Grab,
	Drop,
};

UENUM(BlueprintType)
enum class EIF_VRInteractionInputType: uint8
{
	None,
	Trigger,
	Grip,
	Thumbstick_X,
	Thumbstick_Neg_X,
	Thumbstick_Y,
	Thumbstick_Neg_Y,
	Menu_1,
	Menu_2,
	System
};


UENUM(BlueprintType)
enum class EIF_VRInteractionInputEvent: uint8
{
	None,
	Press,
	Release,
	DoubleClick
};

UENUM(BlueprintType)
enum class EIF_VRMoveDirection: uint8
{
	Camera,
	CustomAimComponent,
};
UENUM(BlueprintType)
enum class EIF_VRTraceType: uint8
{
	None,
	Parabola,
	Linear,
};

UENUM(BlueprintType)
enum class EIF_VRHandType: uint8
{
	None,
	Left,
	Right,
};


UENUM(BlueprintType)
enum class EIF_2HandGrabMainHandRightAxis: uint8
{
	X,
	Y,
	Z,
};



USTRUCT(BlueprintType)
struct FIF_VRInteractionInputData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EIF_VRInteractionInputType Type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EIF_VRInteractionInputEvent Event;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bTouch = false;
	
};



UCLASS(BlueprintType, Blueprintable, Config = Game, Abstract,EditInlineNew)
class UIF_InputTypeConfig : public UObject
{
	GENERATED_BODY()
	
};

UCLASS(BlueprintType, Blueprintable, Config = Game, Abstract,EditInlineNew)
class UIF_EventConfig : public UObject
{
	GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable, Config = Game, EditInlineNew, DisplayName=String)
class UIF_EventConfig_String : public UIF_EventConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=String)
	FString EventName = TEXT("Custom");
};
UCLASS(BlueprintType, Blueprintable, Config = Game, EditInlineNew, DisplayName=MovementType)
class UIF_EventConfig_MovementType : public UIF_EventConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Movement)
	EIF_VRMovementType MovementType = EIF_VRMovementType::None;
};



UCLASS(BlueprintType, Blueprintable, Config = Game, Abstract,EditInlineNew, DisplayName=CustomEvent)
class UIF_InputTypeConfig_CustomEvent : public UIF_InputTypeConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category=Event)
	UIF_EventConfig* EventConfig = nullptr;
};


UCLASS(ClassGroup=InteractionFramework, config=Game, DisplayName=Action)
class UIF_InputTypeConfig_CustomEvent_Action : public UIF_InputTypeConfig_CustomEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Action)
	FName Action = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Action)
	TEnumAsByte<EInputEvent> KeyEvent;
	
};

UCLASS(ClassGroup=InteractionFramework, config=Game, DisplayName=Axis)
class UIF_InputTypeConfig_CustomEvent_Axis : public UIF_InputTypeConfig_CustomEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Axis)
	FName Axis = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Axis)
	EIF_MoveAxisFunction Function = EIF_MoveAxisFunction::Greater;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Axis)
	float Value;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Axis)
	bool bAbsValue = false;
	
};


UCLASS(ClassGroup=InteractionFramework, config=Game, DisplayName=Key)
class UIF_InputTypeConfig_CustomEvent_Key : public UIF_InputTypeConfig_CustomEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Key)
	FKey Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Key)
	TEnumAsByte<EInputEvent> KeyEvent;
	
};