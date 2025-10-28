#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitscanWeapon.generated.h"

UCLASS(Blueprintable)
class TRAININGROOM_API UHitscanWeapon : public UWeapon
{
	GENERATED_BODY()

public:
	virtual FWeaponFireResult PerformFire(AActor* Instigator,FVector MuzzleLocation, FVector FireDirection) override;
};
