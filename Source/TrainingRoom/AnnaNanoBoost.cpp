#include "AnnaNanoBoost.h"
#include "Anna.h"
#include "TargetingAbility.h"
#include "TargetingComponent.h"

void UAnnaNanoBoost::Execute(AActor* Instigator)
{
	if (!Instigator) return;
	UTargetingComponent* TargetComp = Instigator->FindComponentByClass<UTargetingComponent>();
	AHeroCharacter* Target;
	/*
	if (TargetComp) 
	{
		UE_LOG(LogTemp, Display, TEXT("NANO INSTIGATOR WAY"));
		Target = TargetComp->GetCurrentTarget();

	}
	*/

	//else
	//{
		UE_LOG(LogTemp, Display, TEXT("NANO CAST ANNA"));
		AAnna* OwnerAnna = Cast<AAnna>(Instigator);
		if (!OwnerAnna)
		{
			UE_LOG(LogTemp,Warning,TEXT("OwnerAnna NULL"));
			return;
		}
		Target = OwnerAnna->GetTargetingComponent() ? OwnerAnna->GetTargetingComponent()->GetCurrentTarget() : nullptr;
	//}

	if (Target && Instigator->GetClass()->ImplementsInterface(UTargetingAbility::StaticClass()))
	{
		// only execute when Target is not a nullptr
		ITargetingAbility::Execute_ApplyEffectToTarget(Instigator, Target);
		Super::Execute(Instigator);
	}

}
