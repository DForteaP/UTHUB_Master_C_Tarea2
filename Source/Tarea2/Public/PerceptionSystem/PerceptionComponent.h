#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerceptionComponent.generated.h"

USTRUCT(BlueprintType)
struct TAREA2_API FPerceptionInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float Radius = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TSubclassOf<AActor> ActorClassToDetect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToDetect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TArray<AActor*> ActorsToIgnore;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAREA2_API UPerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPerceptionComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Perception")
	void DetectActorsInRadius();

	UFUNCTION(BlueprintCallable, Category = "Perception")
	void DrawDebugDetectionSphere() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection", meta = (AllowPrivateAccess = "true"))
	FPerceptionInfo DetectionParams;
	
	UPROPERTY(VisibleAnywhere, Category = "Detection")
	TArray<AActor*> DetectedActors;
};