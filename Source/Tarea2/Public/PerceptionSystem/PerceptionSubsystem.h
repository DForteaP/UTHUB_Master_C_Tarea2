// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PerceptionSubsystem.generated.h"


UCLASS()
class TAREA2_API UPerceptionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterPerceptionComponent(class UPerceptionComponent* Component);
	void UnregisterPerceptionComponent(class UPerceptionComponent* Component);
	
	virtual void Tick(float DeltaTime);
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

private:
	TArray<class UPerceptionComponent*> PerceptionComponents;
};

