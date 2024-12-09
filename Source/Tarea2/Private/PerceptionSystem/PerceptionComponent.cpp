#include "PerceptionSystem/PerceptionComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "PerceptionSystem/Detections/Hearing.h"
#include "PerceptionSystem/Detections/Sight.h"
#include "PerceptionSystem/Detections/Smell.h"

UPerceptionComponent::UPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	PrimaryDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PrimaryDetectionSphere"));
	PrimaryDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PrimaryDetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PrimaryDetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PrimaryDetectionSphere->SetGenerateOverlapEvents(true);
	
	ExtendedDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExtendedDetectionSphere"));
	ExtendedDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExtendedDetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExtendedDetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ExtendedDetectionSphere->SetGenerateOverlapEvents(true);

	SenseMap.Add(ESenseType::Sight, ASight::StaticClass());
	SenseMap.Add(ESenseType::Hearing, AHearing::StaticClass());
	SenseMap.Add(ESenseType::Smell, ASmell::StaticClass());
}

void UPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	DetectedActors.Empty();
	AActor* Owner = GetOwner();
	USceneComponent* RootComponent = Owner ? Owner->GetRootComponent() : nullptr;

	if (RootComponent)
	{
		PrimaryDetectionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		PrimaryDetectionSphere->SetSphereRadius(DetectionParams.Radius);
		PrimaryDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PrimaryDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UPerceptionComponent::HandleBeginOverlapPrimary);

		ExtendedDetectionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		ExtendedDetectionSphere->SetSphereRadius(DetectionParams.ExtendedRadius);
		ExtendedDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ExtendedDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UPerceptionComponent::HandleEndOverlapExtended);
	}

	if (!DetectionParams.ActorsToIgnore.Contains(GetOwner()))
	{
		DetectionParams.ActorsToIgnore.Add(GetOwner());
	}
}

void UPerceptionComponent::InitPerceptionInfo(float Radius, float ExtendedRadius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypesToDetect)
{
	DetectionParams.Radius = Radius;
	DetectionParams.ExtendedRadius = ExtendedRadius;
	DetectionParams.ObjectTypesToDetect = ObjectTypesToDetect;
	
	if (PrimaryDetectionSphere)
	{
		PrimaryDetectionSphere->SetSphereRadius(DetectionParams.Radius);
	}

	if (ExtendedDetectionSphere)
	{
		ExtendedDetectionSphere->SetSphereRadius(DetectionParams.ExtendedRadius);
	}
}

void UPerceptionComponent::SetPerceptionEnabled()
{
	DetectionParams.bPerceptionEnabled = true;
}

void UPerceptionComponent::SetPerceptionDisabled()
{
	DetectionParams.bPerceptionEnabled = false;
	
	for (AActor* Actor : DetectedActors)
	{
		if (Actor)
		{
			OnActorLost.Broadcast(this, Actor);
		}
	}
	DetectedActors.Empty();
}

void UPerceptionComponent::HandleBeginOverlapPrimary(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DetectionParams.bPerceptionEnabled == false){return;}
	if (!OtherActor || OtherActor == GetOwner()){return; }

	if (!DetectedActors.Contains(OtherActor))
	{
		if (DetectionParams.ActorsToIgnore.Contains(OtherActor))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Actor Ignorado: %s"), *OtherActor->GetName());
			return;
		}

		DetectedActors.Add(OtherActor);
		OnActorDetected.Broadcast(this, OtherActor);
		//UE_LOG(LogTemp, Log, TEXT("Actor Detectado: %s"), *OtherActor->GetName());
	}
}

void UPerceptionComponent::HandleEndOverlapExtended(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
	{
		return;
	}
	
	if (OtherActor && DetectedActors.Contains(OtherActor))
	{
		DetectedActors.Remove(OtherActor);
		OnActorLost.Broadcast(this, OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("Actor Perdido: %s"), *OtherActor->GetName());
	}
}

void UPerceptionComponent::InitializeSenses()
{
	InstantiatedSenseImplementations.Empty();
	for (ESenseType SenseType : SenseTypes)
	{
		if (SenseMap.Contains(SenseType))
		{
			TSubclassOf<AUSenseImplementationBase> SenseClass = SenseMap[SenseType];
			if (SenseClass)
			{
				AUSenseImplementationBase* NewSense = GetWorld()->SpawnActor<AUSenseImplementationBase>(SenseClass);
				if (NewSense)
				{
					InstantiatedSenseImplementations.Add(NewSense);
					NewSense->PerformDetection();
					UE_LOG(LogTemp, Log, TEXT("Sentido activado: %s"), *NewSense->GetName());
				}
			}
		}
	}
}