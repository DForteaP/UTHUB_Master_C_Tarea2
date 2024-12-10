// Fill out your copyright notice in the Description page of Project Settings.


#include "PerceptionSystem/Detections/Sight.h"


// Sets default values
ASight::ASight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<AActor*> ASight::PerformDetection_Implementation()
{

	UE_LOG(LogTemp, Warning, TEXT("Viendo"));
	
	return Super::PerformDetection_Implementation();
}

