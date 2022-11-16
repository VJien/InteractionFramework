// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IFTypes.generated.h"

UENUM(BlueprintType)
enum class EMoveAxisFunction: uint8
{
	Equal,
	Greater,
	GreaterOrEqual,
	Less,
	LessOrEqual,
};

UENUM(BlueprintType)
enum class EVRMovementType: uint8
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
enum class EVRPlayerAction: uint8
{
	None,
	Move,
	Grab,
	Drop,
};

UENUM(BlueprintType)
enum class EVRInteractionInputType: uint8
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
enum class EVRInteractionInputEvent: uint8
{
	None,
	Press,
	Release,
	DoubleClick
};


USTRUCT(BlueprintType)
struct FVRInteractionInputData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EVRInteractionInputType Type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EVRInteractionInputEvent Event;
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

UCLASS(BlueprintType, Blueprintable, Config = Game, EditInlineNew)
class UIF_EventConfig_String : public UIF_EventConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	FString EventName = TEXT("Custom");
};
UCLASS(BlueprintType, Blueprintable, Config = Game, EditInlineNew)
class UIF_EventConfig_MovementType : public UIF_EventConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	EVRMovementType MovementType = EVRMovementType::None;
};



UCLASS(BlueprintType, Blueprintable, Config = Game, Abstract,EditInlineNew)
class UIF_InputTypeConfig_CustomEvent : public UIF_InputTypeConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent, Instanced)
	UIF_EventConfig* Event = nullptr;
};


UCLASS(ClassGroup=InteractionFramework, config=Game)
class UIF_InputTypeConfig_CustomEvent_Action : public UIF_InputTypeConfig_CustomEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	FName Action = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	TEnumAsByte<EInputEvent> KeyEvent;
	
};

UCLASS(ClassGroup=InteractionFramework, config=Game)
class UIF_InputTypeConfig_CustomEvent_Axis : public UIF_InputTypeConfig_CustomEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	FName Axis = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	EMoveAxisFunction Function = EMoveAxisFunction::Greater;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	float Value;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	bool bAbsValue = false;
	
};


UCLASS(ClassGroup=InteractionFramework, config=Game)
class UIF_InputTypeConfig_CustomEvent_Key : public UIF_InputTypeConfig_CustomEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	FKey Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CustomEvent)
	TEnumAsByte<EInputEvent> KeyEvent;
	
};