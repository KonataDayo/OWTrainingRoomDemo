#include "TracerRecall.h"
#include "Tracer.h"
#include "GameFramework/Actor.h"

void UTracerRecall::Execute(AActor* Instigator)
{
	Super::Execute(Instigator);
	// should be playing a RECALL ANIMATION
	if (!Instigator) return;
	UE_LOG(LogTemp,Warning,TEXT("Recall Activated"));
	if (RecallInfos.IsValidIndex(0))
	{
		Instigator->SetActorLocation(RecallInfos[0].Location);
		Instigator->SetActorRotation(RecallInfos[0].Rotation);
		if (auto Tracer = Cast<ATracer>(Instigator))
		{
			Tracer->SetCurrentHP(FMath::Max(RecallInfos[0].HP,Tracer->GetCurrentHP()));
		}
	}
}

void UTracerRecall::SetLocationAndRotation(FVector Location, FRotator Rotation)
{
	LocationThreeSecAgo = Location;
	RotationThreeSecAgo = Rotation;
}
