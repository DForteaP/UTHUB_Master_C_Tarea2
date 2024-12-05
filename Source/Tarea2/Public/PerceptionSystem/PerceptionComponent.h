// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerceptionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAREA2_API UPerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPerceptionComponent();
	
	void PerformDetection();
	virtual ~UPerceptionComponent() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Perception")
	float DetectionRadius = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Perception")
	float DetectionInterval = 1.0f;
	
	float TimeSinceLastDetection = 0.0f;
};