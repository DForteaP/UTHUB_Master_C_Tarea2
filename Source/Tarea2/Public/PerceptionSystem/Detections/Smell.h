#pragma once

#include "CoreMinimal.h"
#include "SenseImplementationBase.h"
#include "Smell.generated.h"

UCLASS()
class TAREA2_API ASmell : public AUSenseImplementationBase
{
	GENERATED_BODY()

public:
	ASmell();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual TArray<AActor*> PerformDetection_Implementation() override;
};
