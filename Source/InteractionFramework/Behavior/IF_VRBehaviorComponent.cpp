// Fill out your copyright notice in the Description page of Project Settings.


#include "IF_VRBehaviorComponent.h"

#include "IF_Behavior.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UIF_VRBehaviorComponent::UIF_VRBehaviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UIF_VRBehaviorComponent::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);
	for (auto P: FunctionMap_LeftHand)
	{
		P.Value.Unbind();
	}
	for (auto P: FunctionMap_RightHand)
	{
		P.Value.Unbind();
	}
}


// Called when the game starts
void UIF_VRBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	InitFunctionMap();
	
}

void UIF_VRBehaviorComponent::InitFunctionMap()
{
	FunctionMap_LeftHand.Empty();
	FunctionMap_RightHand.Empty();

	BIND_BEHAVIOR(MoveForward);
	BIND_BEHAVIOR(MoveRight);
	BIND_BEHAVIOR(Teleport);
	BIND_BEHAVIOR(Turn);
	BIND_BEHAVIOR(Grab);
	BIND_BEHAVIOR(Release);
	BIND_BEHAVIOR(Menu1);
	BIND_BEHAVIOR(Menu2);
	BIND_BEHAVIOR(Use);
	BIND_BEHAVIOR(Custom1);
	BIND_BEHAVIOR(Custom2);
	BIND_BEHAVIOR(Custom3);
	BIND_BEHAVIOR(Select1);
	BIND_BEHAVIOR(Select2);
}


// Called every frame
void UIF_VRBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UIF_VRBehaviorComponent::AddBehaviorInput(EIF_VRHandType Hand, EIF_VRInputType Input,
	EIF_VRPlayerBehavior Behavior, int32 Priority, bool bTriggerOnce)
{
	if (Hand == EIF_VRHandType::None || Input == EIF_VRInputType::None)
	{
		return false;
	}
	auto& Curr = ProxyMap.FindOrAdd(Input);
	if (Curr.Num() == 0)
	{
		Curr.Emplace(FIF_BehaviorProxy(Priority,Behavior,Hand,bTriggerOnce));
	}
	else
	{
		Curr.Emplace(FIF_BehaviorProxy(Priority,Behavior,Hand,bTriggerOnce));
		Curr.Sort([](FIF_BehaviorProxy A, FIF_BehaviorProxy B)
		{
			return A.Priority < B.Priority;
		});
	}
	return false;
}

void UIF_VRBehaviorComponent::ProcessBehaviorEvent(EIF_VRHandType Hand, EIF_VRInputType Input, float Value)
{
	if (Hand == EIF_VRHandType::None)
	{
		return;
	}
	if (Input == EIF_VRInputType::None)
	{
		return;
	}
	if (auto Proxy = ProxyMap.Find(Input))
	{
		if (Proxy->Num() > 0)
		{
			const auto Last =Proxy->Last();
			if (Last.Hand == EIF_VRHandType::Left)
			{
				if(auto Func = FunctionMap_LeftHand.Find(Last.Behavior))
				{
					Func->Execute(Value);
					if (Last.bTriggerOnce)
					{
						Proxy->RemoveAt(Proxy->Num()-1);
					}
					return;
				}
			}
			else if (Last.Hand == EIF_VRHandType::Right)
			{
				if(auto Func = FunctionMap_RightHand.Find(Last.Behavior))
				{
					Func->Execute(Value);
					if (Last.bTriggerOnce)
					{
						Proxy->RemoveAt(Proxy->Num()-1);
					}
					return;
				}
			}
		}
	}

	if (BehaviorData)
	{
		EIF_VRPlayerBehavior* pBehavior = nullptr;
		if (Hand == EIF_VRHandType::Left)
		{
			pBehavior =  BehaviorData->Left.Find(Input);
			if (!pBehavior)
			{
				return;
			}
			auto Func = FunctionMap_LeftHand.Find(*pBehavior);
			if (Func)
			{
				Func->Execute(Value);
			}
		}
		else
		{
			pBehavior =  BehaviorData->Right.Find(Input);
			if (!pBehavior)
			{
				return;
			}
			auto Func = FunctionMap_RightHand.Find(*pBehavior);
			if (Func)
			{
				Func->Execute(Value);
			}
		}
		
	}
}
