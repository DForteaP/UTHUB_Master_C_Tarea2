#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PerceptionSubsystem.generated.h"


UCLASS()
class TAREA2_API UPerceptionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:

	
private:

	void GetAllPerceptionComponentsOwners(TArray<AActor*>& ActorsPerception) const;
	void RegisterNewActor(AActor* InNewActor) const;

	UPROPERTY()
	TArray<class AActor*>& ActorsPerception;
	
};


