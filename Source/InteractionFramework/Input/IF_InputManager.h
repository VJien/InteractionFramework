// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/InputSettings.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "UObject/Object.h"
#include "IF_InputManager.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIONFRAMEWORK_API UIF_InputManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<EIF_VRHMDType,UIF_InputConfig*> InputMapping;


	TWeakObjectPtr<UInputSettings> InputSettings = nullptr;
};
