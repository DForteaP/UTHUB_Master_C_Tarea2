#pragma once

#include "CoreMinimal.h"
#include "SenseImplementationBase.h"
#include "Hearing.generated.h"


UCLASS()
class TAREA2_API AHearing : public AUSenseImplementationBase
{
	GENERATED_BODY()

public:
	AHearing();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual TArray<AActor*> PerformDetection_Implementation() override;
	
};
