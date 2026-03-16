#pragma once
#include "CoreMinimal.h"
#include "WeaponFireResult.h"
#include "UObject/NoExportTypes.h"
#include "FireStrategy.generated.h"

class UWeapon;

UCLASS(Abstract)
class TRAININGROOM_API UFireStrategy : public UObject
{
	GENERATED_BODY()

public:
	virtual FWeaponFireResult ExecuteFire(AActor* Instigator,UWeapon* Weapon ,FVector MuzzleLocation, FVector FireDirection)
		PURE_VIRTUAL(UFireStrategy::ExecuteFire,return FWeaponFireResult(););

	virtual void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
