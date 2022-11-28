// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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
enum class EIF_GrabStat: uint8
{
	None = 0,
	Main = 1,
	Secondary = 2,
	MarkForRelease = 3,
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
	NegX,
	NegY,
	NegZ,
	Custom,
	Lock
};

UENUM(BlueprintType)
enum class EIF_VRGrabRule: uint8
{
	Any,
	AlwaysMainHand,
	AlwaysSecondaryHand,
};

UENUM(BlueprintType)
enum class EIF_VRHMDType: uint8
{
	None,
	SteamVR_Vive,
	SteamVR_Index,
	SteamVR_Cosmos,
	Meta_Rift,
	Meta_Quest,
	WindowsVR,
	PSVR,
	Pico4
};

UENUM(BlueprintType)
enum class EIF_HandFingerType: uint8
{
	None,
	Thumbstick,
	Index,
	Middle,
	Ring,
	Pinky,

};


USTRUCT(BlueprintType)
struct FIF_VRHandFingerData
{
	GENERATED_BODY()
public:
	bool IsNealyEqual(FIF_VRHandFingerData Other, float BlendTolerance = 0.01, float RotationTolerance = 0.01);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMax = 1,ClampMin=0))
	float GrabBlend = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation_Joint1 = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation_Joint2 = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation_Joint3 = FRotator::ZeroRotator;
};
USTRUCT(BlueprintType)
struct FIF_VRHandPoseData: public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIF_VRHandFingerData Thumbstick;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIF_VRHandFingerData Index;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIF_VRHandFingerData Middle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIF_VRHandFingerData Ring;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIF_VRHandFingerData Pinky;
};

USTRUCT(BlueprintType)
struct FIF_VRHandMeshPosition
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location_Left = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation_Left = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location_Right = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation_Right = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location_Aim = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation_Aim = FRotator::ZeroRotator;
};

UCLASS(BlueprintType)
class UIF_VRHandMeshPositionData: public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_VRHMDType, FIF_VRHandMeshPosition> Data;
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