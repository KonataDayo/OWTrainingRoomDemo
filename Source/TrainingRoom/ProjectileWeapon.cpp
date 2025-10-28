#include "ProjectileWeapon.h"
#include "HeroCharacter.h"
#include "Projectile.h"
#include "WeaponFireResult.h"
#include "Engine/World.h"

FWeaponFireResult UProjectileWeapon::PerformFire(AActor* Instigator,FVector MuzzleLocation, FVector FireDirection)
{
	Super::PerformFire(Instigator,MuzzleLocation, FireDirection);
	FWeaponFireResult Result;
	APawn* InstigatorPawn = Cast<APawn>(Instigator);
	if (!BulletClass || !InstigatorPawn) return Result;
	// Projectile Logic Below
	Result.WeaponFireType = EWeaponFireType::WFT_Projectile;
	FActorSpawnParameters Params;
	Params.Instigator = InstigatorPawn;
	Params.Owner = Instigator;
	// only responsible for spawning projectile, binding etc. is excluded
	AProjectile* thisProjectile = GetWorld()->SpawnActor<AProjectile>(BulletClass, MuzzleLocation,FireDirection.GetSafeNormal().Rotation(),Params);
	if (thisProjectile)
	{
		thisProjectile->SetIsSkill(false);
		thisProjectile->SetInstigator(InstigatorPawn);
		Result.Projectile = thisProjectile;
	}
	else UE_LOG(LogTemp,Warning,TEXT("[ProjectileWeapon]%s Failed to spawn projectile"),*Instigator->GetName());
	return Result;
}
