// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IF_PoolItem.generated.h"





// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIF_PoolItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONFRAMEWORK_API IIF_PoolItem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InteractionFramework | Pool")
	void OnRegister();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InteractionFramework | Pool")
	void OnReset();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InteractionFramework | Pool")
	void MarkDelete();
	
};
