#include "Tarea2/Public/PerceptionSystem/PerceptionComponent.h"
#include "Tarea2/Public/PerceptionSystem/PerceptionSubsystem.h"

UPerceptionComponent::UPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		if (UPerceptionSubsystem* Subsystem = GetWorld()->GetSubsystem<UPerceptionSubsystem>())
		{
			
		}
	}
}