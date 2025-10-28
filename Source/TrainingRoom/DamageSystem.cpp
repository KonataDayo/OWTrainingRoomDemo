#include "DamageSystem.h"
#include "HeroCharacter.h"
#include "Weapon.h"

void UDamageSystem::ApplyDamage(AActor* Instigator,AActor* Target, FDamageData& DamageData)
{
	if (!Target) return;
	if (Target->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		DamageData.FinalDamage = CalculateDamageWithDecay(DamageData.Weapon,DamageData.HitDistance);
		IDamageable::Execute_TakeDamage(Target,DamageData);
	}
}

float UDamageSystem::CalculateDamageWithDecay(UWeapon* InstigatorWeapon, float HitDistance)
{
	check(InstigatorWeapon);
	float DecayDistance = InstigatorWeapon->GetDecayDistance();
	float OriginalDamage = InstigatorWeapon->GetDamage();
	float DecayRatio = InstigatorWeapon->GetDecayRatio();
	if (DecayDistance == 0 || HitDistance <= DecayDistance) return OriginalDamage;
	return OriginalDamage * DecayRatio;

}
