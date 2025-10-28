#include "AnnaNanoBoost.h"
#include "Anna.h"
#include "TargetingAbility.h"
#include "TargetingComponent.h"

void UAnnaNanoBoost::Execute(AActor* Instigator)
{
	AAnna* OwnerAnna = Cast<AAnna>(Instigator);
	if (!OwnerAnna)
	{
		UE_LOG(LogTemp,Warning,TEXT("OwnerAnna NULL"));
		return;
	}
	AHeroCharacter* Target = OwnerAnna->GetTargetingComponent() ? OwnerAnna->GetTargetingComponent()->GetCurrentTarget() : nullptr;
	if (Target && OwnerAnna->GetClass()->ImplementsInterface(UTargetingAbility::StaticClass()))
	{
		ITargetingAbility::Execute_ApplyEffectToTarget(OwnerAnna,Target);
		// only execute when Target is not a nullptr
		Super::Execute(Instigator);
	}
}
