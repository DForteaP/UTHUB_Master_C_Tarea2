// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SenseImplementationBase.h"
#include "Hearing.generated.h"

UCLASS()
class TAREA2_API AHearing : public AUSenseImplementationBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHearing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
