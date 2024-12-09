#include "PerceptionSystem/PerceptionComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

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
