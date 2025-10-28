#pragma once
#include "CoreMinimal.h"
#include "WeaponFireResult.h"
#include "Weapon.generated.h"

/**
 * LINE TRACING IS IMPLEMENTED IN THIS CLASS!
 */

UCLASS(Blueprintable)
class TRAININGROOM_API UWeapon : public UObject
{
	GENERATED_BODY()

public:

	FORCEINLINE float GetDamage() const;

	FORCEINLINE float GetDecayDistance() const;

	FORCEINLINE float GetDecayRatio() const;

	virtual FWeaponFireResult PerformFire(AActor* Instigator ,FVector MuzzleLocation, FVector FireDirection);
protected:

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float DecayDistance = 0.f; // 0 means no decay

	UPROPERTY(EditAnywhere)
	float DecayRatio; // (0,1)

	UPROPERTY(EditAnywhere)
	float Range;

};
