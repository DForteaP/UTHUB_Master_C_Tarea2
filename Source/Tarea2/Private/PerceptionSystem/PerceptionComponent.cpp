#include "PerceptionSystem/PerceptionComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"

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

	AttachToSocket();
	InitializeSenses();
	GetWorld()->GetTimerManager().SetTimer(SenseUpdateTimer, this, &UPerceptionComponent::UpdateSenses, 1.5f, true);
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
			OnActorLost.Broadcast(this, Actor, "Desactivacion");
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
		OnActorDetected.Broadcast(this, OtherActor, "Sentido general");
		//UE_LOG(LogTemp, Log, TEXT("Actor Detectado: %s"), *OtherActor->GetName());
	}
}

void UPerceptionComponent::HandleEndOverlapExtended(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor){return;}
	if (OtherActor && DetectedActors.Contains(OtherActor))
	{
		DetectedActors.Remove(OtherActor);
		OnActorLost.Broadcast(this, OtherActor, "Sentido general");
		//UE_LOG(LogTemp, Warning, TEXT("Actor Perdido: %s"), *OtherActor->GetName());
	}
}

void UPerceptionComponent::InitializeSenses()
{
	for (TSubclassOf<AUSenseImplementationBase> SenseType : SenseTypes)
	{
		if (!SenseType) continue;
		
		AUSenseImplementationBase* SenseInstance = GetWorld()->SpawnActor<AUSenseImplementationBase>(SenseType);
		if (SenseInstance)
		{
			SenseInstance->SetOwner(GetOwner());
			SenseInstance->OnActorDetected.AddDynamic(this, &UPerceptionComponent::HandleActorDetectedFromSense);
			SenseInstance->OnActorLost.AddDynamic(this, &UPerceptionComponent::HandleActorLostFromSense);
			ActiveSenses.Add(SenseInstance);

			UE_LOG(LogTemp, Log, TEXT("Sentido activado: %s"), *SenseType->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No se pudo crear una instancia del sentido: %s"), *SenseType->GetName());
		}
	}
}

void UPerceptionComponent::UpdateSenses()
{
	for (AUSenseImplementationBase* Sense : ActiveSenses)
	{
		if (Sense)
		{
			TArray<AActor*> DetectedActorsSense = Sense->PerformDetection();
			for (AActor* Actor : DetectedActorsSense)
			{
				if (!DetectedActors.Contains(Actor))
				{
					DetectedActors.Add(Actor);
					OnActorDetected.Broadcast(this, Actor, "");
				}
			}
		}
	}
}

void UPerceptionComponent::AddSense(TSubclassOf<AUSenseImplementationBase> SenseType)
{
	if (SenseTypes.Contains(SenseType)){return;}
	SenseTypes.Add(SenseType);
}
void UPerceptionComponent::RemoveSense(TSubclassOf<AUSenseImplementationBase> SenseType)
{
	SenseTypes.Remove(SenseType);
}

void UPerceptionComponent::HandleActorDetectedFromSense(AActor* DetectedActor, FString Sense)
{
	DetectedActors.Add(DetectedActor);
	OnActorDetected.Broadcast(this, DetectedActor, Sense);
}

void UPerceptionComponent::HandleActorLostFromSense(AActor* LostActor, const FString Sense)
{
	DetectedActors.Remove(LostActor);
	OnActorLost.Broadcast(this, LostActor, Sense);
}

void UPerceptionComponent::AttachToSocket()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		USkeletalMeshComponent* SkeletalMeshComp = OwnerActor->FindComponentByClass<USkeletalMeshComponent>();
		if (SkeletalMeshComp)
		{
			
			this->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), DetectionParams.SocketName);
			UE_LOG(LogTemp, Log, TEXT("Componente enganchado al socket %s"), *DetectionParams.SocketName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("El actor no tiene un SkeletalMeshComponent."));
		}
	}
}