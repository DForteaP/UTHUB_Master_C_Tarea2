#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SenseImplementationBase.generated.h"


UCLASS()
class TAREA2_API AUSenseImplementationBase : public AActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sense")
	TArray<AActor*> PerformDetection();
};
