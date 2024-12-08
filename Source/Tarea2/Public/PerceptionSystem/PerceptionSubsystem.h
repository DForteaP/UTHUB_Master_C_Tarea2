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

private:
	UFUNCTION(BlueprintCallable)
	void GetAllPerceptionComponents(TArray<UPerceptionComponent*>& OutActorsPerceptionComponents) const;

	UFUNCTION()
	void RegisterNewActor(AActor* InNewActor);

	UPROPERTY(VisibleAnywhere)
	TArray<UPerceptionComponent*> ActorsPerceptionComponents;

	FDelegateHandle OnActorSpawnedDelegateHandle;
};

