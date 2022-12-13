// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/PlayerInput.h"
#include "UObject/Object.h"
#include "IFTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(IF_Log, Log, All);

#define IF_LOG_P(l,...) UE_LOG(IF_Log, Log, TEXT(l), ...)
#define IF_LOG(l) UE_LOG(IF_Log, Log, TEXT(l))
#define IF_WARNING_P(l,...) UE_LOG(IF_Log, Warning, TEXT(l), ...)
#define IF_WARNING(l) UE_LOG(IF_Log, Warning, TEXT(l))
#define IF_ERROR_P(l,...) UE_LOG(IF_Log, Error, TEXT(l), ...)
#define IF_ERROR(l) UE_LOG(IF_Log, Error, TEXT(l))

UENUM(BlueprintType)
enum class EIF_DragType: uint8
{
	None = 0,
	Linear_X,
	Linear_Y,
	Linear_Z,
	Rotation_X,
	Rotation_Y,
	Rotation_Z,
	//相对目标点的转动角度
	Angle_X,
	//相对目标点的转动角度
	Angle_Y,
	//相对目标点的转动角度
	Angle_Z,
	Free
};

UENUM(BlueprintType)
enum class EIF_Direction: uint8
{
	None = 0,
	Front,
	Left,
	Right,
	Back,
	FrontLeft,
	FrontRight,
	BackLeft,
	BackRight
};


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

	SteamVR,
	ValveIndex,
	Vive,
	ViveCosmos,
	ViveFocus,
	ViveFocus3,
	OculusQuestHMD,
	OculusHMD,
	WindowsMR,
	PicoNeo3,
	PicoNeo4,
	PSVR,
	Unknown
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

UENUM(BlueprintType)
enum class EIF_HandFingerStat: uint8
{
	None,
	Idle,
	StatAway,
	Touch,
	Pressed,
	
};



USTRUCT(BlueprintType)
struct FIF_VRHandFingerData
{
	GENERATED_BODY()
public:
	bool IsNealyEqual(FIF_VRHandFingerData Other, float BlendTolerance = 0.01, float RotationTolerance = 0.01);
	FIF_VRHandFingerData(){}
	FIF_VRHandFingerData(float InGrabBlend, FRotator InRotation_Joint1, FRotator InRotation_Joint2, FRotator InRotation_Joint3):
	GrabBlend(InGrabBlend), Rotation_Joint1(InRotation_Joint1), Rotation_Joint2(InRotation_Joint2),Rotation_Joint3(InRotation_Joint3)
	{}

	
	FIF_VRHandFingerData Lerp(FIF_VRHandFingerData Other, float Alpha) const
	{
		return FIF_VRHandFingerData(FMath::Lerp(GrabBlend, Other.GrabBlend,Alpha), FMath::Lerp(Rotation_Joint1, Other.Rotation_Joint1,Alpha),
			FMath::Lerp(Rotation_Joint2, Other.Rotation_Joint2,Alpha),FMath::Lerp(Rotation_Joint3, Other.Rotation_Joint3,Alpha));
	};
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


UENUM(BlueprintType)
enum class EIF_VRInputType: uint8
{
	None,
	Trigger,
	Trigger_Touch,
	Trigger_Axis,
	Grip,
	Grip_Touch,
	Grip_Axis,
	Menu_1,
	Menu_1_Touch,
	Menu_2,
	Menu_2_Touch,
	System,
	Thumbstick_X,
	Thumbstick_Y,
	Thumbstick_Left,
	Thumbstick_Right,
	Thumbstick_Up,
	Thumbstick_Down,
	Thumbstick,
	Thumbstick_Touch
};
UENUM(BlueprintType)
enum class EIF_VRPlayerBehavior: uint8
{
	None,
	MoveForward,
	MoveRight,
	Teleport,
	Turn,
	Grab,
	Release,
	Menu1,
	Menu2,
	Use,
	Select,
	SelectUp,
	SelectDown,
	SelectRight,
	SelectLeft,
	Custom1,
	Custom2,
	Custom3
};

USTRUCT(BlueprintType)
struct FIF_InputActionMapping
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInputActionKeyMapping> Keys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EIF_VRPlayerBehavior Behavior = EIF_VRPlayerBehavior::None;
	
	
};
USTRUCT(BlueprintType)
struct FIF_InputAxisMapping
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EIF_VRInputType Input = EIF_VRInputType::None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EIF_VRHandType Hand = EIF_VRHandType::Left;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInputAxisKeyMapping> Keys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EIF_VRPlayerBehavior Behavior = EIF_VRPlayerBehavior::None;
	
};

UCLASS(ClassGroup=InteractionFramework, config=Game)
class UIF_InputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_VRInputType, FIF_InputActionMapping> ActionMapping_Left;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_VRInputType, FIF_InputActionMapping> ActionMapping_Right;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_VRInputType, FIF_InputAxisMapping>  AxisMapping_Left;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_VRInputType, FIF_InputAxisMapping>  AxisMapping_Right;
};




UCLASS(ClassGroup=InteractionFramework, config=Game)
class UIF_VRBehaviorDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_VRInputType, EIF_VRPlayerBehavior> Left;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_VRInputType, EIF_VRPlayerBehavior> Right;

};


USTRUCT(BlueprintType)
struct FIF_FingerPoseTableKey
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_HandFingerStat, FName> TableKey;
	
	
};


UCLASS(ClassGroup=InteractionFramework, config=Game,BlueprintType)
class UIF_VRFingerStatDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_HandFingerType, FIF_FingerPoseTableKey> Data;
	
};


UCLASS(ClassGroup=InteractionFramework, config=Game, BlueprintType)
class UIF_VRHMDFingerStatDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EIF_VRHMDType, UIF_VRFingerStatDataAsset*> Data;
	
};