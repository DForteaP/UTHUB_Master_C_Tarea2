#include "PerceptionSystem/Detections/SenseImplementationBase.h"

TArray<AActor*> AUSenseImplementationBase::PerformDetection_Implementation()
{

	TArray<AActor*> DetectedActors;
	
	ProcessDetectionResults(DetectedActors);

	return DetectedActors;

}

void AUSenseImplementationBase::ProcessDetectionResults(const TArray<AActor*>& CurrentlyDetectedActors)
{
	for (AActor* Actor : CurrentlyDetectedActors)
	{
		if (!PreviouslyDetectedActors.Contains(Actor))
		{
			OnActorDetected.Broadcast(Actor);
		}
	}
	
	for (AActor* Actor : PreviouslyDetectedActors)
	{
		if (!CurrentlyDetectedActors.Contains(Actor))
		{
			OnActorLost.Broadcast(Actor);
		}
	}
	
	PreviouslyDetectedActors = CurrentlyDetectedActors;
}