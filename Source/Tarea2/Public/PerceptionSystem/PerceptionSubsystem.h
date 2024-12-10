#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PerceptionSubsystem.generated.h"

class UPerceptionComponent;

UCLASS()
class TAREA2_API UPerceptionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:
	
	UFUNCTION(BlueprintCallable)
	TArray<UPerceptionComponent*> GetComponentsRegistered();

private:
	UFUNCTION(BlueprintCallable)
	void GetAllPerceptionComponents(TArray<UPerceptionComponent*>& OutActorsPerceptionComponents) const;
	
	UFUNCTION()
	void RegisterNewActor(AActor* InNewActor);
	
	UFUNCTION()
	void SubscribeToPerceptionComponent(UPerceptionComponent* PerceptionComponent);

	UFUNCTION(BlueprintCallable)
	void SetPerceptionEnabledForActors(const TArray<UPerceptionComponent*>& AffectedComponents, bool bEnablePerception);

	UPROPERTY(VisibleAnywhere)
	TArray<UPerceptionComponent*> ActorsPerceptionComponents;

	UPROPERTY(VisibleAnywhere, Blueprintable)
	TArray<UPerceptionComponent*> RegisteredPerceptionComponents;

	FDelegateHandle OnActorSpawnedDelegateHandle;
	
	void SubscribeToPerceptionComponents();

	UFUNCTION()
	void HandleActorDetected(UPerceptionComponent* PerceptionComponent, AActor* DetectedActor, FString Sense);

	UFUNCTION()
	void HandleActorLost(UPerceptionComponent* PerceptionComponent, AActor* LostActor, FString Sense);

};

