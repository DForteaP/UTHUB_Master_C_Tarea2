#pragma once

#include "CoreMinimal.h"
#include "SenseImplementationBase.h"
#include "Sight.generated.h"

UCLASS()
class TAREA2_API ASight : public AUSenseImplementationBase
{
	GENERATED_BODY()

public:
	ASight();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual TArray<AActor*> PerformDetection_Implementation() override;

	FString Sense = "VISTA";
};
