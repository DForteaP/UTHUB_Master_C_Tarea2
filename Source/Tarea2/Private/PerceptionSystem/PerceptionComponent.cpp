#include "Tarea2/Public/PerceptionSystem/PerceptionComponent.h"
#include "Tarea2/Public/PerceptionSystem/PerceptionSubsystem.h"

UPerceptionComponent::UPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		if (UPerceptionSubsystem* Subsystem = GetWorld()->GetSubsystem<UPerceptionSubsystem>())
		{
			Subsystem->RegisterPerceptionComponent(this);
		}
	}
}

void UPerceptionComponent::PerformDetection()
{
	TArray<AActor*> DetectedActors;
	FVector Location = GetOwner()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(DetectionRadius);
	
	GetWorld()->SweepMultiByChannel(DetectedActors,Location,Location,FQuat::Identity,ECC_Pawn,Sphere);

	for (AActor* Actor : DetectedActors)
	{
		if (Actor->FindComponentByClass<UPerceptionComponent>()) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor detectado: %s"), *Actor->GetName());
		}
	}
}

UPerceptionComponent::~UPerceptionComponent()
{
	if (GetWorld())
	{
		if (UPerceptionSubsystem* Subsystem = GetWorld()->GetSubsystem<UPerceptionSubsystem>())
		{
			Subsystem->UnregisterPerceptionComponent(this);
		}
	}
}