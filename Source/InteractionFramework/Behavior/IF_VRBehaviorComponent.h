// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IF_Behavior.h"
#include "InteractionFramework/Component/Base/IF_ActorComponent.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IF_VRBehaviorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBehavior, EIF_VRPlayerBehavior, Behavior, float, Value, EIF_VRHandType, Hand);
DECLARE_DELEGATE_OneParam( FBehaviorEvent, float );


#define BEHAVIOR_FUNCTION(BehaviorName) \
void Left##BehaviorName(float Value) \
{ \
	EIF_VRHandType HandType =  EIF_VRHandType::Left; \
	if (UKismetSystemLibrary::DoesImplementInterface(GetOwner(), UIF_Behavior::StaticClass())) \
	{ \
		IIF_Behavior::Execute_##BehaviorName(GetOwner(),Value, HandType); \
	} \
	OnBehaviorActive.Broadcast(EIF_VRPlayerBehavior::BehaviorName, Value , HandType); \
}; \
void Right##BehaviorName(float Value) \
{ \
EIF_VRHandType HandType = EIF_VRHandType::Right; \
if (UKismetSystemLibrary::DoesImplementInterface(GetOwner(), UIF_Behavior::StaticClass())) \
{ \
IIF_Behavior::Execute_##BehaviorName(GetOwner(),Value, HandType); \
} \
OnBehaviorActive.Broadcast(EIF_VRPlayerBehavior::BehaviorName, Value , HandType); \
};


#define BIND_BEHAVIOR(BehaviorName) \
FBehaviorEvent L_##BehaviorName; \
FBehaviorEvent R_##BehaviorName; \
L_##BehaviorName.BindUObject(this,&UIF_VRBehaviorComponent::Left##BehaviorName); \
FunctionMap_LeftHand.Add(EIF_VRPlayerBehavior::BehaviorName,   L_##BehaviorName);	\
R_##BehaviorName.BindUObject(this,&UIF_VRBehaviorComponent::Right##BehaviorName); \
FunctionMap_RightHand.Add(EIF_VRPlayerBehavior::BehaviorName,   R_##BehaviorName);	






USTRUCT()
struct FIF_BehaviorProxy
{
	GENERATED_BODY()
	FIF_BehaviorProxy(){}
	FIF_BehaviorProxy(int32 InPriority, EIF_VRPlayerBehavior InBehavior, EIF_VRHandType InHand, bool bInTriggerOnce):
	Priority(InPriority), Behavior(InBehavior), Hand(InHand), bTriggerOnce(bInTriggerOnce)
	{}
	virtual ~FIF_BehaviorProxy(){}
	
	virtual bool operator ==(const FIF_BehaviorProxy& Other) const
	{
		return Priority == Other.Priority && Behavior ==Other.Behavior && Hand == Other.Hand && bTriggerOnce == Other.bTriggerOnce;
	}
	virtual bool operator!=(const FIF_BehaviorProxy& Other) const
	{
		return !(*this == Other);
	}
	UPROPERTY()
	int32 Priority = 0;
	UPROPERTY()
	EIF_VRPlayerBehavior Behavior = EIF_VRPlayerBehavior::None;
	UPROPERTY()
	EIF_VRHandType Hand = EIF_VRHandType::None;
	UPROPERTY()
	bool bTriggerOnce = false;
};

USTRUCT()
struct FIF_BehaviorProxyContainer
{
	GENERATED_BODY()
	
	UPROPERTY()
	EIF_VRInputType Input = EIF_VRInputType::None;
	UPROPERTY()
	TArray<FIF_BehaviorProxy> Proxies;

};





UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONFRAMEWORK_API UIF_VRBehaviorComponent : public UIF_ActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIF_VRBehaviorComponent();
	virtual void DestroyComponent(bool bPromoteChildren) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void InitFunctionMap();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//添加扩展输入, 会覆盖默认输入,  越大的Priority先执行
	UFUNCTION(BlueprintCallable)
	bool AddExtendBehaviorInput(EIF_VRHandType Hand, EIF_VRInputType Input, EIF_VRPlayerBehavior Behavior, int32 Priority = 0,bool bTriggerOnce = false, bool bUnique = true);
	//移除匹配的所有输入
	UFUNCTION(BlueprintCallable)
	bool RemoveExtendBehaviorInput(EIF_VRHandType Hand, EIF_VRInputType Input, EIF_VRPlayerBehavior Behavior);
	//移除最高优先级的输入
	UFUNCTION(BlueprintCallable)
	bool RemoveTopExtendBehaviorInput(EIF_VRHandType Hand, EIF_VRInputType Input);
	//移除所有相关输入
	UFUNCTION(BlueprintCallable)
	bool RemoveAllExtendBehaviorInput(EIF_VRHandType Hand, EIF_VRInputType Input);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBehaviorValue(EIF_VRPlayerBehavior Behavior);

	
	UFUNCTION(BlueprintCallable)
	void ProcessBehaviorEvent(EIF_VRHandType Hand, EIF_VRInputType Input, float Value);

	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=Config)
	UIF_VRBehaviorDataAsset* BehaviorData = nullptr;


protected:
	UPROPERTY(BlueprintAssignable)
	FOnBehavior OnBehaviorActive;

	
	TMap<EIF_VRPlayerBehavior, FBehaviorEvent> FunctionMap_LeftHand;
	TMap<EIF_VRPlayerBehavior, FBehaviorEvent> FunctionMap_RightHand;
	
	TMap<EIF_VRInputType, TArray<FIF_BehaviorProxy>> ExtendInputMap;

	
	BEHAVIOR_FUNCTION(MoveForward);
	BEHAVIOR_FUNCTION(MoveRight);
	BEHAVIOR_FUNCTION(Teleport);
	BEHAVIOR_FUNCTION(Turn);
	BEHAVIOR_FUNCTION(Grab);
	BEHAVIOR_FUNCTION(Release);
	BEHAVIOR_FUNCTION(Menu1);
	BEHAVIOR_FUNCTION(Menu2);
	BEHAVIOR_FUNCTION(Use);
	BEHAVIOR_FUNCTION(Select);
	BEHAVIOR_FUNCTION(SelectUp);
	BEHAVIOR_FUNCTION(SelectDown);
	BEHAVIOR_FUNCTION(SelectLeft)
	BEHAVIOR_FUNCTION(SelectRight)
	BEHAVIOR_FUNCTION(Custom1);
	BEHAVIOR_FUNCTION(Custom2);
	BEHAVIOR_FUNCTION(Custom3);

	


	
};

