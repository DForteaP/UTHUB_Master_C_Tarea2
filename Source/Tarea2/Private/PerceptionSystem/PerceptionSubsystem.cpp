#include "Tarea2/Public/PerceptionSystem/PerceptionSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Tarea2/Public/PerceptionSystem/PerceptionComponent.h"

void UPerceptionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GetAllPerceptionComponentsOwners(ActorsPerception);

	FOnActorSpawned::FDelegate Delegate;
	Delegate.BindUObject(this, &ThisClass::RegisterNewActor);
	FDelegateHandle Handle = GetWorld()->AddOnActorSpawnedHandler(Delegate);
}

void UPerceptionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UPerceptionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UPerceptionSubsystem::GetAllPerceptionComponentsOwners(TArray<AActor*>& PercepCompOwners) const
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);

	for (auto* Actor : OutActors)
	{
		if (Actor->FindComponentByClass<UPerceptionComponent>())
		{
			PercepCompOwners.Add(Actor);
		}
	}
}

void UPerceptionSubsystem::RegisterNewActor(AActor* InNewActor) const
{
	ActorsPerception.Add(InNewActor);
}

