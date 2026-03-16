#pragma once
#include "CoreMinimal.h"
#include "FireStrategy.h"
#include "ProjectileFireStrategy.generated.h"

UCLASS()
class TRAININGROOM_API UProjectileFireStrategy : public UFireStrategy
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> BulletClass;

	FTransform CalculateCrosshair(AActor* Instigator) const;

	virtual FWeaponFireResult DoLaunch(AActor* Instigator, FVector MuzzleLocation, FVector FireDirection);
public:
	FWeaponFireResult ExecuteFire(AActor* Instigator,UWeapon* Weapon, FVector MuzzleLocation, FVector FireDirection) override;

	virtual void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
