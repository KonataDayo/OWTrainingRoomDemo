#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DamageSystem.generated.h"

class UWeapon;
struct FDamageData;

UCLASS()
class TRAININGROOM_API UDamageSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void ApplyDamage(AActor* Instigator,AActor* Target, FDamageData& DamageData);

	float CalculateDamageWithDecay(UWeapon* InstigatorWeapon,float HitDistance);
};
