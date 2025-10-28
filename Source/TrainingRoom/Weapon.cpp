#include "Weapon.h"

float UWeapon::GetDamage() const
{
	return Damage;
}

float UWeapon::GetDecayDistance() const
{
	return DecayDistance;
}

float UWeapon::GetDecayRatio() const
{
	return DecayRatio;
}

FWeaponFireResult UWeapon::PerformFire(AActor* Instigator,FVector MuzzleLocation, FVector FireDirection)
{
	FWeaponFireResult f;
	return f;
}


