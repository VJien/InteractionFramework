// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionFramework/Data/IFTypes.h"
#include "Kismet/KismetStringLibrary.h"
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

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
	static FIF_VRHandFingerData LerpFingerData(FIF_VRHandFingerData A, FIF_VRHandFingerData B, float Alpha);
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
	static FIF_VRHandFingerData GetFingerData(FIF_VRHandPoseData PoseData, EIF_HandFingerType Finger);

	
	static FString InputTypeToString(EIF_VRInputType InputType);
	static EIF_VRInputType StringToInputType(FString String);

	template <typename TEnum>
	static FString IFEnumToString(const FString& EnumName, TEnum Enum, bool bShortName = true)
	{
		const UEnum* ep = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (ep)
		{
			if (bShortName)
			{
				FString tempStr = ep->GetNameByValue((int)Enum).ToString();
				TArray<FString> arr = UKismetStringLibrary::ParseIntoArray(tempStr, TEXT("::"));
				if (arr.Num() > 0)
				{
					return arr.Last();
				}
			}

			return ep->GetNameByValue((int)Enum).ToString();
		}
		return TEXT("");
	}

	template <typename TEnum>
	static TEnum IFStringToEnum(const FString& EnumName, FString EnumValue)
	{
		const UEnum* ep = FindObject<UEnum>(ANY_PACKAGE, *EnumName, 1);
		if (!ep)
		{
			return TEnum(0);
		}
		return (TEnum)ep->GetIndexByName(FName(*FString(EnumValue)));
	}


};

