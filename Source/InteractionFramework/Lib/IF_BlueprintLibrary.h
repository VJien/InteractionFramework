// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "UObject/Object.h"
#include "IF_BlueprintLibrary.generated.h"


UCLASS()
class INTERACTIONFRAMEWORK_API UIF_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TEnumAsByte<EWorldType::Type> GetWorldType(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
	static bool IsPIE_Mode(UObject* WorldContext);
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
	static bool IsEditorMode(UObject* WorldContext);
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
	static bool IsGameMode(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
	static EIF_VRHMDType GetHMDType();


};

