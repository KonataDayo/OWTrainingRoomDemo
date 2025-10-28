#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "WeaponFireResult.generated.h"

UENUM(BlueprintType)
enum class EWeaponFireType : uint8
{
	WFT_Hitscan UMETA(DisplayName = "Hitscan"),
	WFT_Projectile UMETA(DisplayName = "Projectile"),
	WFT_NONE
};

USTRUCT(BlueprintType)
struct FWeaponFireResult
{
	GENERATED_BODY()

	UPROPERTY()
	EWeaponFireType WeaponFireType = EWeaponFireType::WFT_NONE;

	// for hit-scan weapon
	UPROPERTY()
	TArray<FHitResult> HitResults;

	// the projectile spawned
	UPROPERTY()
	class AProjectile* Projectile;
};

class TRAININGROOM_API WeaponFireResult
{
public:
	WeaponFireResult();
	~WeaponFireResult();
};
