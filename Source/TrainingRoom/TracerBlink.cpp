#include "TracerBlink.h"
#include "GameFramework/Controller.h"

void UTracerBlink::Execute(AActor* Instigator)
{
	Super::Execute(Instigator);
	if (!Instigator) return;
	if (auto PC = Instigator->GetInstigatorController())
	{
		FVector Dir = FVector::ZeroVector;
		float ForwardValue = PC->GetInputAxisValue("MoveForward");
		float RightValue = PC->GetInputAxisKeyValue("MoveRight");
		Dir.X = ForwardValue;
		Dir.Y = RightValue;
		if (!Dir.IsNearlyZero())
		{
			Dir.Normalize();
		}
		FVector NewLocation = Instigator->GetActorLocation() + Dir * 750.f;
		Instigator->SetActorLocation(NewLocation);
	}
}
