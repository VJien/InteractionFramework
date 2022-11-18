#pragma once
#include "CoreMinimal.h"
#include "UIF_InputManager.generated.h"



UCLASS(BlueprintType)
class UIF_InputManger : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	
};