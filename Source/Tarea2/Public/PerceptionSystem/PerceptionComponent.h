#pragma once

#include "CoreMinimal.h"
#include "Detections/SenseImplementationBase.h"
#include "Components/ActorComponent.h"
#include "PerceptionComponent.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorDetected, UPerceptionComponent*, PerceptionComponent, AActor*, DetectedActor, FString, PerceptionType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorLost, UPerceptionComponent*, PerceptionComponent, AActor*, LostActor, FString, PerceptionType);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	FName SocketName =TEXT("Socket");
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAREA2_API UPerceptionComponent : public USceneComponent
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

	UPROPERTY()
	TArray<AUSenseImplementationBase*> ActiveSenses;

	FTimerHandle SenseUpdateTimer;
	
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
	void UpdateSenses();

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

	UFUNCTION()
	void HandleActorDetectedFromSense(AActor* DetectedActor,  FString Sense);

	UFUNCTION()
	void HandleActorLostFromSense(AActor* LostActor,  FString Sense);
	void AttachToSocket();
};