#include "PerceptionSystem/Detections/Smell.h"

ASmell::ASmell()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASmell::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASmell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<AActor*> ASmell::PerformDetection_Implementation()
{
    //UE_LOG(LogTemp, Warning, TEXT("Oliendo"));
    TArray<AActor*> DetectedActors;
    
    UWorld* World = GetWorld();
    if (!World){return DetectedActors;}
    
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor){return DetectedActors;}
    
    FVector StartLocation = OwnerActor->GetActorLocation();
    float SmellRadius = 300.f;
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerActor);
    QueryParams.bTraceComplex = true;
    
    TArray<FHitResult> HitResults;
    bool bHit = World->SweepMultiByChannel(HitResults,StartLocation,StartLocation, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SmellRadius),QueryParams);
    
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
    DrawDebugSphere(World, StartLocation, SmellRadius, 12, FColor::Green, false, 1.0f);
#endif

    ProcessDetectionResults(DetectedActors, Sense);
    
    return DetectedActors;
}