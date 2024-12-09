#include "Tarea2/Public/PerceptionSystem/PerceptionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Tarea2/Public/PerceptionSystem/PerceptionComponent.h"


void UPerceptionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FOnActorSpawned::FDelegate Delegate = FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::RegisterNewActor);
	OnActorSpawnedDelegateHandle = GetWorld()->AddOnActorSpawnedHandler(Delegate);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		GetAllPerceptionComponents(ActorsPerceptionComponents);
		SubscribeToPerceptionComponents();
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
		SubscribeToPerceptionComponent(PerceptionComp);
	}
}

void UPerceptionSubsystem::SubscribeToPerceptionComponent(UPerceptionComponent* PerceptionComponent)
{
		if (PerceptionComponent)
		{
			PerceptionComponent->OnActorDetected.AddDynamic(this, &UPerceptionSubsystem::HandleActorDetected);
			PerceptionComponent->OnActorLost.AddDynamic(this, &UPerceptionSubsystem::HandleActorLost);
		}
}

void UPerceptionSubsystem::SubscribeToPerceptionComponents()
{
	for (UPerceptionComponent* PerceptionComponent : ActorsPerceptionComponents)
		{
			if (PerceptionComponent)
			{
				PerceptionComponent->OnActorDetected.AddDynamic(this, &UPerceptionSubsystem::HandleActorDetected);
				PerceptionComponent->OnActorLost.AddDynamic(this, &UPerceptionSubsystem::HandleActorLost);
			}
		}
}

void UPerceptionSubsystem::HandleActorDetected(AActor* DetectedActor)
{
	UE_LOG(LogTemp, Log, TEXT("Subsystem: Actor Detected: %s"), *DetectedActor->GetName());
	
}

void UPerceptionSubsystem::HandleActorLost(AActor* LostActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Subsystem: Actor Lost: %s"), *LostActor->GetName());


}