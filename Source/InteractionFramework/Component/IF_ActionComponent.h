// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/IF_ActorComponent.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "IF_ActionComponent.generated.h"


DECLARE_dynamic_mu



#define INPUT_EVENT(PropertyName) \
FORCEINLINE void Action##PropertyName() \
{ \
int32 P = ActionFuncs.Find(&UIF_ActionComponent::Action##PropertyName); \
	if (P>=0 && P<12) \
{ \
ReceiveActionEvent(ActionEvent[P]); \
} \
else \
{UE_LOG(LogTemp, Warning, TEXT("Can not find Action Function"));}  \
}; \
FORCEINLINE void Axis##PropertyName(float NewVal) \
{ \
int32 P = AxisFuncs.Find(&UIF_ActionComponent::Axis##PropertyName); \
if (P>=0 && P<12 && AxisConfigObj[P]) \
{ \
if (CheckAxisValue(NewVal, AxisConfigObj[P])) \
	ReceiveAxisEvent(AxisEvent[P], NewVal); \
} \
else \
{UE_LOG(LogTemp, Warning, TEXT("Can not find Axis Function"));}  \
};



UCLASS(ClassGroup=(InteractionFramework), meta=(BlueprintSpawnableComponent),Blueprintable, BlueprintType)
class INTERACTIONFRAMEWORK_API UIF_ActionComponent : public UIF_ActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_ActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void InitInputMap();
	

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category=config)
	TArray<UIF_InputTypeConfig_CustomEvent*> InputMap;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReceiveActionEvent( UIF_EventConfig* Event);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReceiveAxisEvent(UIF_EventConfig* Event, float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckAxisValue(float Value, UIF_InputTypeConfig_CustomEvent_Axis* AxisConfig);
	
	int32 CurrBindIndex_Action = 0;
	int32 CurrBindIndex_Axis = 0;
	
	//默认创建12个事件
	INPUT_EVENT(0)
	INPUT_EVENT(1)
	INPUT_EVENT(2)
	INPUT_EVENT(3)
	INPUT_EVENT(4)
	INPUT_EVENT(5)
	INPUT_EVENT(6)
	INPUT_EVENT(7)
	INPUT_EVENT(8)
	INPUT_EVENT(9)
	INPUT_EVENT(10)
	INPUT_EVENT(11)

	int32 MaxEventNum = 12;
	
	TWeakObjectPtr<UInputComponent> InputComponent = nullptr;
	
	TArray<void(UIF_ActionComponent::*)()> ActionFuncs;
	TArray<FName> ActionName;
	TArray<UIF_EventConfig*> ActionEvent;
	TArray<UIF_InputTypeConfig_CustomEvent_Action*> ActionConfigObj;
	
	TArray<void(UIF_ActionComponent::*)(float)> AxisFuncs;
	TArray<FName> AxisName;
	TArray<UIF_EventConfig*> AxisEvent;
	TArray<UIF_InputTypeConfig_CustomEvent_Axis*> AxisConfigObj;


	bool bHasInit = false;
	
};
