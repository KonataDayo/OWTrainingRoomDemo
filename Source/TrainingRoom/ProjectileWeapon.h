#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

UCLASS()
class TRAININGROOM_API UProjectileWeapon : public UWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> BulletClass;

public:
	virtual FWeaponFireResult PerformFire(AActor* Instigator,FVector MuzzleLocation, FVector FireDirection) override;
};
