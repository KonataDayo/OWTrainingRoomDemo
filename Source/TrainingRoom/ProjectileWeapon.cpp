#include "ProjectileWeapon.h"
#include "DamageSystem.h"
#include "HeroCharacter.h"
#include "ObjectPool.h"
#include "ObjectPoolSubsystem.h"
#include "Projectile.h"
#include "TimerManager.h"
#include "WeaponFireResult.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

void UProjectileWeapon::HandleProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OverlappedComponent) return;
	AActor* ProjectileActor = OverlappedComponent->GetOwner();
	if (!ProjectileActor) return;
	// check cached subsystems
	if (!CachedDamageSubsystem && GetWorld()) CachedDamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDamageSystem>();
	if (!CachedObjectPoolSubsystem && GetWorld()) CachedObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();

	if (AActor* Instigator = ProjectileActor->GetInstigator())
	{
		// avoid overlapping with ourselves
		if (OtherActor != Instigator)
		{
			float HitDistance = CalculateDistance(Instigator, OtherActor);
			FDamageData thisData(Instigator, this, HitDistance);
			CachedObjectPoolSubsystem->Return(ProjectileActor);
			CachedDamageSubsystem->ApplyDamage(Instigator, OtherActor, thisData);
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("[WeaponComponent]%s Failed to call GetOwner()"), *OverlappedComponent->GetName());
}

FWeaponFireResult UProjectileWeapon::PerformFire(AActor* Instigator,FVector MuzzleLocation, FVector FireDirection)
{
	FWeaponFireResult Result;
	APawn* InstigatorPawn = Cast<APawn>(Instigator);
	if (!BulletClass || !InstigatorPawn) return Result;
	
	// Projectile Logic Below
	Result.WeaponFireType = EWeaponFireType::WFT_Projectile;
	// only responsible for spawning projectile, binding etc. is excluded
	auto thisProjectile = GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->Get<AProjectile>(BulletClass);
	if (thisProjectile)
	{
		thisProjectile->SetIsSkill(false);
		thisProjectile->SetInstigator(InstigatorPawn);
		Result.Projectile = thisProjectile;
		thisProjectile->SetActorLocationAndRotation(MuzzleLocation,FireDirection.GetSafeNormal().Rotation());
		thisProjectile->InitializeVelocity(FireDirection);
	}
	return Result;
}

FWeaponFireResult UProjectileWeapon::PerformAuxiliaryFire(AActor* Instigator, FVector MuzzleLocation,
	FVector FireDirection)
{
	FWeaponFireResult Result;
	APawn* InstigatorPawn = Cast<APawn>(Instigator);
	if (!BulletClass || !InstigatorPawn) return Result;

	// Projectile Logic Below
	for (int32 i = 0; i < 3; i++)
	{
		Result.WeaponFireType = EWeaponFireType::WFT_Projectile;
		// only responsible for spawning projectile, binding etc. is excluded
		auto thisProjectile = GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->Get<AProjectile>(BulletClass);
		if (thisProjectile)
		{
			thisProjectile->SetIsSkill(false);
			thisProjectile->SetInstigator(InstigatorPawn);
			Result.Projectile = thisProjectile;
			thisProjectile->SetActorLocationAndRotation(MuzzleLocation, FireDirection.GetSafeNormal().Rotation());
			thisProjectile->InitializeVelocity(FireDirection);
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[this]()
			{
				UE_LOG(LogTemp, Warning, TEXT("[%s] Delay finished"),*GetName());
			},
			2.0f,
			false
		);
	}

	return Result;
}

void UProjectileWeapon::HandleHit(AActor* Instigator, const FWeaponFireResult& FireResult,
                                  UDamageSystem* DamageSystem)
{
	Super::HandleHit(Instigator, FireResult,DamageSystem);
	AProjectile* Projectile = FireResult.Projectile;
	if (!Instigator || !DamageSystem || !Projectile) return;
	if (!CachedObjectPoolSubsystem && GetWorld()) CachedObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	
	if (UBoxComponent* HB = Projectile->GetHitBox())
	{
		HB->OnComponentBeginOverlap.AddDynamic(this,&UProjectileWeapon::HandleProjectileOverlap);
	}
}

