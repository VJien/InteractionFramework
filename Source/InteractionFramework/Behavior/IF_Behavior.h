// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "UObject/Object.h"
#include "IF_Behavior.generated.h"

/*
None,
MoveForward,
MoveRight,
Teleport,
Turn,
Grab,
Drop,
Menu1,
Menu2,
Use,
Custom1,
Custom2,
Custom3
*/

UINTERFACE(MinimalAPI)
class UIF_Behavior : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONFRAMEWORK_API IIF_Behavior
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void MoveForward(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void MoveRight(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Teleport(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Turn(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Grab(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Release(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Menu1(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Menu2(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Use(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Custom1(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Custom2(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Custom3(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Select1(float Value, EIF_VRHandType Hand);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Behavior)
	void Select2(float Value, EIF_VRHandType Hand);


	
};
