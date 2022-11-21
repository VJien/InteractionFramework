// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionFramework/Component/Base/IF_ActorComponent.h"
#include "UIF_EnhancedActionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONFRAMEWORK_API UUIF_EnhancedActionComponent : public UIF_ActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUIF_EnhancedActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;




	


	
};
