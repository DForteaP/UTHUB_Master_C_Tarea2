#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerceptionComponent.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorDetected, UPerceptionComponent*, PerceptionComponent, AActor*, DetectedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorLost, UPerceptionComponent*, PerceptionComponent, AActor*, LostActor);

USTRUCT(BlueprintType)
struct TAREA2_API FPerceptionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float Radius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float ExtendedRadius = 750.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TSubclassOf<AActor> ActorClassToDetect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToDetect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	bool bPerceptionEnabled = true;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAREA2_API UPerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPerceptionComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnActorDetected OnActorDetected;

	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnActorLost OnActorLost;
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Perception")
	void SetPerceptionEnabled();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Perception")
	void SetPerceptionDisabled();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection", meta = (AllowPrivateAccess = "true"))
	FPerceptionInfo DetectionParams;

	UPROPERTY(VisibleAnywhere, Category = "Detection")
	TArray<AActor*> DetectedActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	USphereComponent* PrimaryDetectionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	USphereComponent* ExtendedDetectionSphere;

	UFUNCTION()
	void HandleBeginOverlapPrimary(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlapExtended(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};