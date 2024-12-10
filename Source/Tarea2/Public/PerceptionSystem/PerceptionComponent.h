#pragma once

#include "CoreMinimal.h"
#include "Detections/SenseImplementationBase.h"
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
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToDetect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	bool bPerceptionEnabled = true;
};

UENUM(BlueprintType)
enum class ESenseType : uint8
{
	Sight UMETA(DisplayName = "Sight"),
	Hearing UMETA(DisplayName = "Hearing"),
	Smell UMETA(DisplayName = "Smell")
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
	
	//PROPERTIES
	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnActorDetected OnActorDetected;

	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnActorLost OnActorLost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sense")
	TSet<TSubclassOf<AUSenseImplementationBase>> SenseTypes;

	//FUNCTIONS

	//Cambiar parametros percepción esfera
	UFUNCTION(BlueprintCallable, Category = "Perception")
	void InitPerceptionInfo(float Radius, float ExtendedRadius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypesToDetect);

	//Activar percepción
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Perception")
	void SetPerceptionEnabled();

	//Desactivar percepción
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Perception")
	void SetPerceptionDisabled();

	//Inicializar sentidos en lista
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Sense")
	void InitializeSenses();

	//Añadir sentido a lista
	UFUNCTION(Blueprintable, Category = "Perception")
	void AddSense(TSubclassOf<AUSenseImplementationBase> SenseType);

	//Eliminar sentido en lista
	UFUNCTION(Blueprintable, Category = "Perception")
	void RemoveSense(TSubclassOf<AUSenseImplementationBase> SenseType);
	
private:

	//PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection", meta = (AllowPrivateAccess = "true"))
	FPerceptionInfo DetectionParams;

	UPROPERTY(VisibleAnywhere, Category = "Detection")
	TArray<AActor*> DetectedActors;

	UPROPERTY()
	USphereComponent* PrimaryDetectionSphere;

	UPROPERTY()
	USphereComponent* ExtendedDetectionSphere;

	//FUNCTIONS
	UFUNCTION()
	void HandleBeginOverlapPrimary(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlapExtended(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};