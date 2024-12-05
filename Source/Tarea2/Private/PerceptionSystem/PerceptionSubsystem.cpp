#include "Tarea2/Public/PerceptionSystem/PerceptionSubsystem.h"
#include "Tarea2/Public/PerceptionSystem/PerceptionComponent.h"

void UPerceptionSubsystem::RegisterPerceptionComponent(UPerceptionComponent* Component)
{
	if (Component && !PerceptionComponents.Contains(Component))
	{
		PerceptionComponents.Add(Component);
	}
}

void UPerceptionSubsystem::UnregisterPerceptionComponent(UPerceptionComponent* Component)
{
	PerceptionComponents.Remove(Component);
}

void UPerceptionSubsystem::Tick(float DeltaTime)
{
	for (UPerceptionComponent* Component : PerceptionComponents)
	{
		if (Component)
		{
			Component->PerformDetection();
		}
	}
}