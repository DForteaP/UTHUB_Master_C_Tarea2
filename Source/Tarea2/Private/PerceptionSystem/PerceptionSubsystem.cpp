#include "Tarea2/Public/PerceptionSystem/PerceptionSubsystem.h"

#include "LandscapeGizmoActiveActor.h"
#include "Kismet/GameplayStatics.h"
#include "Tarea2/Public/PerceptionSystem/PerceptionComponent.h"


void UPerceptionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ActorsPerceptionComponents.Empty();
	
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
		if (PerceptionComp && !ActorsPerceptionComponents.Contains(PerceptionComp))
		{
			OutActorsPerceptionComponents.Add(PerceptionComp);
		}
	}
}

void UPerceptionSubsystem::RegisterNewActor(AActor* InNewActor)
{
	if (!InNewActor){return;}
	UPerceptionComponent* PerceptionComp = InNewActor->FindComponentByClass<UPerceptionComponent>();
	if (PerceptionComp && !ActorsPerceptionComponents.Contains(PerceptionComp))
	{
		ActorsPerceptionComponents.Add(PerceptionComp);
		SubscribeToPerceptionComponent(PerceptionComp);
	}
}

void UPerceptionSubsystem::SubscribeToPerceptionComponent(UPerceptionComponent* PerceptionComponent)
{
		if (PerceptionComponent && !RegisteredPerceptionComponents.Contains(PerceptionComponent))
		{
			PerceptionComponent->OnActorDetected.AddDynamic(this, &UPerceptionSubsystem::HandleActorDetected);
			PerceptionComponent->OnActorLost.AddDynamic(this, &UPerceptionSubsystem::HandleActorLost);
			RegisteredPerceptionComponents.Add(PerceptionComponent);
		}
}

void UPerceptionSubsystem::SubscribeToPerceptionComponents()
{
	for (UPerceptionComponent* PerceptionComponent : ActorsPerceptionComponents)
		{
			if (PerceptionComponent && !RegisteredPerceptionComponents.Contains(PerceptionComponent))
			{
				PerceptionComponent->OnActorDetected.AddDynamic(this, &UPerceptionSubsystem::HandleActorDetected);
				PerceptionComponent->OnActorLost.AddDynamic(this, &UPerceptionSubsystem::HandleActorLost);
				RegisteredPerceptionComponents.Add(PerceptionComponent);
			}
		}
}

void UPerceptionSubsystem::HandleActorDetected(UPerceptionComponent* PerceptionComponent, AActor* DetectedActor)
{
	if (!PerceptionComponent || !DetectedActor) return;

	FString PerceptionOwnerName = PerceptionComponent->GetOwner() ? PerceptionComponent->GetOwner()->GetName() : TEXT("Alguien");
	FString DetectedActorName = DetectedActor->GetName();

	UE_LOG(LogTemp, Log, TEXT("%s ha detectado a %s"), *PerceptionOwnerName, *DetectedActorName);
}

void UPerceptionSubsystem::HandleActorLost(UPerceptionComponent* PerceptionComponent, AActor* LostActor)
{
	if (!PerceptionComponent || !LostActor) return;

	FString PerceptionOwnerName = PerceptionComponent->GetOwner() ? PerceptionComponent->GetOwner()->GetName() : TEXT("Alguien");
	FString LostActorName = LostActor->GetName();

	UE_LOG(LogTemp, Warning, TEXT("%s ha perdido a %s"), *PerceptionOwnerName, *LostActorName);
}

void UPerceptionSubsystem::SetPerceptionEnabledForActors(const TArray<UPerceptionComponent*>& AffectedComponents, bool bEnablePerception)
{
	for (UPerceptionComponent* ComponentAffected : AffectedComponents)
	{
		if (!ComponentAffected)
		{
			UE_LOG(LogTemp, Warning, TEXT("Null component in AffectedComponents array."));
			continue;
		}

		if (RegisteredPerceptionComponents.Contains(ComponentAffected))
		{
			if (bEnablePerception)
			{
				ComponentAffected->SetPerceptionEnabled();
			}
			else
			{
				ComponentAffected->SetPerceptionDisabled();
			}

			UE_LOG(LogTemp, Log, TEXT("Perception %s for component: %s"),
				   bEnablePerception ? TEXT("enabled") : TEXT("disabled"),
				   *ComponentAffected->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Component %s is not registered in the subsystem."),
				   *ComponentAffected->GetName());
		}
	}
}