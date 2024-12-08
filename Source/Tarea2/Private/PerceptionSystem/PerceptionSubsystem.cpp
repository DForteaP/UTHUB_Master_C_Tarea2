#include "Tarea2/Public/PerceptionSystem/PerceptionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Tarea2/Public/PerceptionSystem/PerceptionComponent.h"


void UPerceptionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GetAllPerceptionComponents(ActorsPerceptionComponents);
	
	FOnActorSpawned::FDelegate Delegate = FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::RegisterNewActor);
	OnActorSpawnedDelegateHandle = GetWorld()->AddOnActorSpawnedHandler(Delegate);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		GetAllPerceptionComponents(ActorsPerceptionComponents);
	});
}

void UPerceptionSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (OnActorSpawnedDelegateHandle.IsValid())
	{
		GetWorld()->RemoveOnActorSpawnedHandler(OnActorSpawnedDelegateHandle);
	}

	ActorsPerceptionComponents.Empty();
}

bool UPerceptionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UPerceptionSubsystem::GetAllPerceptionComponents(TArray<UPerceptionComponent*>& OutActorsPerceptionComponents) const
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);
	
	for (AActor* Actor : OutActors)
	{
		UPerceptionComponent* PerceptionComp = Actor->FindComponentByClass<UPerceptionComponent>();
		if (PerceptionComp)
		{
			OutActorsPerceptionComponents.Add(PerceptionComp);
		}
	}
}

void UPerceptionSubsystem::RegisterNewActor(AActor* InNewActor)
{
	UPerceptionComponent* PerceptionComp = InNewActor->FindComponentByClass<UPerceptionComponent>();
	if (PerceptionComp)
	{
		ActorsPerceptionComponents.Add(PerceptionComp);
	}
}