#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetingAbility.generated.h"

/**
 *  COMMAND MODE
 */
class AHeroCharacter;

UENUM()
enum class ETargetOption
{
	ETO_OnlyAllies  UMETA(DisplayName = "Allies Only"),
	ETO_OnlyEnemies UMETA(DisplayName = "Enemies Only"),
	ETO_AllHeroes	UMETA(DisplayName = "All Heroes")
};


UINTERFACE(MinimalAPI,BlueprintType)
class UTargetingAbility : public UInterface
{
	GENERATED_BODY()
};

class TRAININGROOM_API ITargetingAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Targeting")
	void ApplyEffectToTarget(AHeroCharacter* Target);
};
