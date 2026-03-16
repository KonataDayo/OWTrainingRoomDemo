#include "TracerRecall.h"
#include "Tracer.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

void UTracerRecall::Execute(AActor* Instigator)
{
	Super::Execute(Instigator);
	// should be playing a RECALL ANIMATION
	if (!Instigator) return;
#if WITH_EDITOR
	UE_LOG(LogTemp,Warning,TEXT("[%s] Recall Activated"),*GetName());
#endif
	if (RecallInfos.IsValidIndex(0))
	{
		auto PC = Cast<APlayerController>(Instigator->GetInstigatorController());
		if (PC) Instigator->DisableInput(PC);
		else UE_LOG(LogTemp,Error,TEXT("[%s] Cannot Cast to PlayerController"),*GetName());
		Instigator->SetActorLocation(RecallInfos[0].Location);
		Instigator->SetActorRotation(RecallInfos[0].Rotation);
		if (auto Tracer = Cast<ATracer>(Instigator))
		{
			Tracer->SetCurrentHP(FMath::Max(RecallInfos[0].HP,Tracer->GetCurrentHP()));
		}

		if (PC) Instigator->EnableInput(PC);
	}
}

void UTracerRecall::SetLocationAndRotation(FVector Location, FRotator Rotation)
{
	LocationThreeSecAgo = Location;
	RotationThreeSecAgo = Rotation;
}
