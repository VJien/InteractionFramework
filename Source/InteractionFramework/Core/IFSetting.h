// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionFramework/SceneItem/IF_VR_TraceVisual.h"

#include "IFSetting.generated.h"

/**
 * 
 */
UCLASS(Config=IFSetting, defaultconfig, DisplayName = "Interaction Framework Setting")
class INTERACTIONFRAMEWORK_API UIFSetting : public UObject
{
	GENERATED_BODY()
public:
	
	static UIFSetting * Get()
	{
		return GetMutableDefault<UIFSetting>();
	}
	UPROPERTY(EditAnywhere, Category="Pool", Config)
	int32 DefaultPoolNum = 10;
	UPROPERTY(EditAnywhere, Category="Pool", Config)
	TMap<TSoftClassPtr<ACharacter>, int32 > AIPoolData;

	UPROPERTY(EditAnywhere, Category="Pool", Config)
	TSoftClassPtr<AIF_VR_TraceVisual> TraceVisual;
	
};
