#include "PerceptionSystem/Detections/Hearing.h"

AHearing::AHearing()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHearing::BeginPlay()
{
	Super::BeginPlay();
}

void AHearing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<AActor*> AHearing::PerformDetection_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Escuchando"));
	TArray<AActor*> DetectedActors;
	UWorld* World = GetWorld();
	if (!World){return DetectedActors;}
	
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{return DetectedActors;}
	
	FVector StartLocation = OwnerActor->GetActorLocation();
	FVector ForwardDirection = OwnerActor->GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardDirection * 1000);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor); 
	QueryParams.bTraceComplex = true;
	
	TArray<FHitResult> HitResults;
	bool bHit = World->LineTraceMultiByChannel(HitResults,StartLocation,EndLocation,ECC_Pawn,QueryParams);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				APawn* HitPawn = Cast<APawn>(HitActor);
				if (HitPawn && !DetectedActors.Contains(HitActor))
				{
					DetectedActors.Add(HitActor);
				}
			}
		}
	}

#if WITH_EDITOR
	DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, 1.0f, 0, 2.0f);
#endif

	ProcessDetectionResults(DetectedActors, Sense);
	
	return DetectedActors;
}



