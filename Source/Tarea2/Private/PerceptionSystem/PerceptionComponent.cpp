#include "Tarea2/Public/PerceptionSystem/PerceptionComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

UPerceptionComponent::UPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	DetectActorsInRadius();
}

void UPerceptionComponent::DetectActorsInRadius()
{
	DetectedActors.Empty();
	
	UWorld* World = GetWorld();
	AActor* Owner = GetOwner();
	if (!World || !Owner)
	{
		return;
	}
	
	UKismetSystemLibrary::SphereOverlapActors(
		World,
		Owner->GetActorLocation(), DetectionParams.Radius, DetectionParams.ObjectTypesToDetect, DetectionParams.ActorClassToDetect, DetectionParams.ActorsToIgnore, DetectedActors);
}

void UPerceptionComponent::DrawDebugDetectionSphere() const
{
	UWorld* World = GetWorld();
	AActor* Owner = GetOwner();
	if (!World || !Owner)
	{
		return;
	}
	
	DrawDebugSphere(
		World,Owner->GetActorLocation(),DetectionParams.Radius,24,FColor::Blue,false,1.0f,2.0f);
}