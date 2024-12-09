// Fill out your copyright notice in the Description page of Project Settings.


#include "PerceptionSystem/Detections/Hearing.h"


// Sets default values
AHearing::AHearing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHearing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHearing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

