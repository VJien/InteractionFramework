// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/IF_ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionFramework/Core/IFSetting.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "InteractionFramework/SceneItem/IF_VR_TraceVisual.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IF_VR_MoveComponent.generated.h"
USTRUCT(BlueprintType)
struct FIF_VRMoveData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	ACharacter* VR_Pawn = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	UCharacterMovementComponent* MovementComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	USceneComponent* TraceAimComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	UCameraComponent* Camera = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	USceneComponent* MoveDirectionComponent = nullptr;
	
};

UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent), DisplayName=IF_VR_Movement)
class INTERACTIONFRAMEWORK_API UIF_VR_MoveComponent : public UIF_ActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_VR_MoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	ACharacter* VR_Pawn = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	UCharacterMovementComponent* MovementComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	USceneComponent* TraceAimComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	USceneComponent* MoveDirectionComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	UCameraComponent* Camera = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	AIF_VR_TraceVisual* TraceVisual = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	UIFSetting* Settings = nullptr;
	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init(FIF_VRMoveData Data);
	
	//重新校准角色胶囊体与相机的位置， 保证重心一直保持在相机下方
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Common")
	bool bCapsuleAdjust = true;
	
	//移动方向依据
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Move")
	EIF_VRMoveDirection MoveDirection = EIF_VRMoveDirection::Camera;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Move")
	float MoveSpeedScale = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Move")
	bool bKeepHorizonDirection = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport")
	EIF_VRTraceType TeleportTraceType = EIF_VRTraceType::Parabola;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport")
	bool bTeleportTraceByChannel = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport", meta=(EditCondition="bTeleportTraceByChannel"))
	TEnumAsByte<ETraceTypeQuery> TeleportChannel = ETraceTypeQuery::TraceTypeQuery1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport", meta=(EditCondition="!bTeleportTraceByChannel"))
	TArray<TEnumAsByte<EObjectTypeQuery>> TeleportObjectTypes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Parabola")
	float TeleportLaunchSpeed = 500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Parabola")
	float TeleportProjectileRaidus = 5.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport")
	TEnumAsByte<EDrawDebugTrace::Type> TeleportDrawDebugType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Parabola")
	float TeleportDrawDebugTime = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Parabola")
	float TeleportSimFrequency = 15.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Parabola")
	float TeleportMaxSimTime = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Parabola")
	float TeleportOverrideGravityZ = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Nav")
	FVector TeleportProjectNavigationExtent = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Nav")
	float TeleportProjectNavMeshCellHeight = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Linear")
	float TeleportTraceDistance = 10000.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Teleport|Linear")
	float TeleportMaxDistance = 1000.0f;

	
	UPROPERTY(BlueprintReadWrite, Category="Config|Teleport")
	TArray<AActor*> TeleportIgnoreActors;
	bool bTeleportTraceActive = false;
	FVector TeleportProjectedLocation = FVector::ZeroVector;
	TArray<FVector> TeleportPathPoints;
	bool bValidTeleportLocation = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Config|Turn")
	float TurnAngle = 45.f;




	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TurnCharacter(float Value);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveCharacter(float Fwd, float Right);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveCharacterForward(float Fwd);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveCharacterRight(float Right);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AdjustCapsule();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartTeleportTrace();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateTeleportTrace();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FinishAndTryTeleport();
	UFUNCTION()
	bool IsValidTeleportLocation(const FHitResult& HitResult, FVector& ProjectLocation);
	
};


