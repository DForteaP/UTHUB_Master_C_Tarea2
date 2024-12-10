// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SenseImplementationBase.h"
#include "Sight.generated.h"

UCLASS()
class TAREA2_API ASight : public AUSenseImplementationBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual TArray<AActor*> PerformDetection_Implementation() override;
};
