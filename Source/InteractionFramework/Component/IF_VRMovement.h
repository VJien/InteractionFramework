// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/IF_ActorComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/MovementComponent.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "IF_VRMovement.generated.h"


UENUM(BlueprintType)
enum class EIF_WalkDirectionType: uint8
{
	Camera,
	Device,
};

USTRUCT(BlueprintType)
struct FIF_WalkData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EIF_WalkDirectionType Direction = EIF_WalkDirectionType::Camera;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* DirectionComponent = nullptr;
	
};

#define BIND_ACTION( InputComponent,  ActionName,  Event, PropertyName) \
{ \
	auto Bind = InputComponent->BindAction(ActionName, Event, this, &UIF_VRMovement::On##PropertyName##); \
};

#define BIND_AXIS( InputComponent,  AxisName, PropertyName) \
{ \
auto Bind = InputComponent->BindAxis(AxisName, this, &UIF_VRMovement::On##PropertyName##Axis); \
};


#define MOVEMENT_EVENT(PropertyName) \
FORCEINLINE void On##PropertyName() \
{ \
	Do##PropertyName##(1.0); \
}; \
FORCEINLINE void On##PropertyName##Axis(float NewVal) \
{ \
if(CheckAxis(EVRMovementType::PropertyName, NewVal)) \
{ \
Do##PropertyName##(NewVal); \
}; \
};




UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class INTERACTIONFRAMEWORK_API UIF_VRMovement : public UIF_ActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_VRMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	

	void InitInput();
	void BindAction(UInputComponent* InputComponent, UIF_InputTypeConfig_Movement_Action* AC);
	void BindAxis(UInputComponent* InputComponent, UIF_InputTypeConfig_Movement_Axis* AX);

	UFUNCTION(BlueprintCallable)
	void SetupWalkData(FIF_WalkData Data);
	
	
	bool CheckAxis(EVRMovementType Type, float Axis);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoWalk(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoPreTeleport(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoTeleport(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoTurn(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoPreSprint(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoSprint(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoJump(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoPreJumpCurve(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void DoJumpCurve(float Value);


	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category=config)
	TArray<UIF_InputTypeConfig_Movement*> MovementAction;

	MOVEMENT_EVENT(Walk);
	MOVEMENT_EVENT(Teleport);
	MOVEMENT_EVENT(PreTeleport);
	MOVEMENT_EVENT(Sprint);
	MOVEMENT_EVENT(Turn);
	MOVEMENT_EVENT(PreSprint);
	MOVEMENT_EVENT(Jump);
	MOVEMENT_EVENT(PreJumpCurve);
	MOVEMENT_EVENT(JumpCurve);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	FIF_WalkData WalkData;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UMovementComponent> MovementComponent = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UInputComponent> mInputComponent = nullptr;

	
};
