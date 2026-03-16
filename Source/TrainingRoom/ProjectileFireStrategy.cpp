#include "ProjectileFireStrategy.h"
#include "GameFramework/PlayerController.h"
#include "Weapon.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "DamageSystem.h"

FTransform UProjectileFireStrategy::CalculateCrosshair(AActor* Instigator) const
{
	FVector CameraLocation;
	AController* Controller = Instigator->GetInstigatorController();
	if (Controller)
	{
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			FVector CameraDirection;
			int32 x, y;
			PC->GetViewportSize(x, y);
			// Crosshair: center of the screen
			PC->DeprojectScreenPositionToWorld(x*0.5f, y*0.5f, CameraLocation, CameraDirection);
			return FTransform(CameraDirection.Rotation(), CameraLocation);
		}
		FRotator CameraRotation;
		Controller->GetActorEyesViewPoint(CameraLocation, CameraRotation);
		return FTransform(CameraRotation, CameraLocation);
	}
	return FTransform::Identity;
}

FWeaponFireResult UProjectileFireStrategy::DoLaunch(AActor* Instigator, FVector MuzzleLocation, FVector FireDirection)
{
	FWeaponFireResult Result;
	APawn* InstigatorPawn = Cast<APawn>(Instigator);
	if (!BulletClass || !InstigatorPawn) return Result;

	FActorSpawnParameters Params;
	Params.Instigator = InstigatorPawn;
	Params.Owner = Instigator;

	// only responsible for spawning projectile, binding etc. is excluded
	AProjectile* thisProjectile = GetWorld()->SpawnActor<AProjectile>(BulletClass, MuzzleLocation, FireDirection.GetSafeNormal().Rotation(), Params);
	if (thisProjectile)
	{
		thisProjectile->SetIsSkill(false);
		thisProjectile->SetInstigator(InstigatorPawn);
		Result.Projectile = thisProjectile;
	}
	return Result;
}

FWeaponFireResult UProjectileFireStrategy::ExecuteFire(AActor* Instigator,UWeapon* Weapon, 
                                                       FVector MuzzleLocation,FVector FireDirection)
{
	if (!Instigator || !Weapon)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s: Instigator or Weapon NULL"),*GetName());
		return Super::ExecuteFire(Instigator, Weapon, MuzzleLocation, FireDirection);
	}
#if WITH_EDITOR
	UE_LOG(LogTemp,Display,TEXT("[ProjectileFire] Fire!"));
#endif
	Super::ExecuteFire(Instigator, Weapon, MuzzleLocation, FireDirection);
	// Fire function, logic in subordinate classes
	FWeaponFireResult Result = DoLaunch(Instigator,MuzzleLocation,FireDirection);
	// Binding Callback Function
	auto Proj = Result.Projectile;
	if (auto HB = Proj->GetHitBox())
	{
		HB->OnComponentBeginOverlap.AddDynamic(this,&UProjectileFireStrategy::OnHit);
	}
	return Result;
	//return Super::ExecuteFire(Instigator, Weapon, MuzzleLocation, FireDirection);
}

void UProjectileFireStrategy::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnHit(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
#if WITH_EDITOR
	UE_LOG(LogTemp,Display,TEXT("[%s] Handling OnHit Logic"),*GetName());
#endif
	if (!GetWorld()) return;
	auto CachedDamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDamageSystem>();
	if (!CachedDamageSubsystem) return;
	if (!OtherActor || !OverlappedComponent) return;
	AActor* ProjectileActor = OverlappedComponent->GetOwner();
	if (!ProjectileActor) return;
	if (AActor* Instigator = ProjectileActor->GetInstigator())
	{
		// avoid overlapping with ourselves
		if (OtherActor != Instigator)
		{
			float HitDistance = FVector::Dist(Instigator->GetActorLocation(), OtherActor->GetActorLocation());
			//FDamageData thisData(Instigator, Weapon, HitDistance);
			FDamageData thisData(Instigator, nullptr, HitDistance);
			CachedDamageSubsystem->ApplyDamage(Instigator, OtherActor, thisData);
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("[WeaponComponent]%s Failed to call GetOwner()"), *OverlappedComponent->GetName());
}
