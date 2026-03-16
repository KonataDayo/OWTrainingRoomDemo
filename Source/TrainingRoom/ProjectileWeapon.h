#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

class UObjectPool;

UCLASS()
class TRAININGROOM_API UProjectileWeapon : public UWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> BulletClass;

	UPROPERTY()
	class UObjectPoolSubsystem* CachedObjectPoolSubsystem;

	UFUNCTION()
	void HandleProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual FWeaponFireResult PerformFire(AActor* Instigator,FVector MuzzleLocation, FVector FireDirection) override;

	virtual FWeaponFireResult PerformAuxiliaryFire(AActor* Instigator, FVector MuzzleLocation, FVector FireDirection) override;

	virtual void HandleHit(AActor* Instigator, const FWeaponFireResult& FireResult, UDamageSystem* DamageSystem) override;
};
