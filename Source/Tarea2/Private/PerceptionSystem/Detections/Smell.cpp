// Fill out your copyright notice in the Description page of Project Settings.


#include "PerceptionSystem/Detections/Smell.h"


// Sets default values
ASmell::ASmell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

