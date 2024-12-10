#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SenseImplementationBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDetectedSense, AActor*, DetectedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorLostSense, AActor*, LostActor);

UCLASS()
class TAREA2_API AUSenseImplementationBase : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Sense")
	FOnActorDetectedSense OnActorDetected;
	
	UPROPERTY(BlueprintAssignable, Category = "Sense")
	FOnActorLostSense OnActorLost;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sense")
	TArray<AActor*> PerformDetection();

protected:

	UPROPERTY()
	TArray<AActor*> PreviouslyDetectedActors;
	
	void ProcessDetectionResults(const TArray<AActor*>& CurrentlyDetectedActors);
};
