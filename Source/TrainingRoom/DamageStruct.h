#pragma once
#include "CoreMinimal.h"
#include "DamageStruct.generated.h"

class AHeroCharacter;
class UWeapon;
class AController;

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Instigator = nullptr;

	UPROPERTY()
 	UWeapon* Weapon = nullptr;

	float HitDistance = 0.f;

	float FinalDamage = 0.f;

	FDamageData() = default;

	FDamageData(AActor* hero,UWeapon* weapon,float distance)
		: Instigator(hero),Weapon(weapon),HitDistance(distance),FinalDamage(0.f){}

};

USTRUCT(BlueprintType)
struct FDeathData
{
	GENERATED_BODY()

	UPROPERTY()
	AHeroCharacter* SlainHero = nullptr;

	UPROPERTY()
	AController* Instigator = nullptr;

	UPROPERTY()
	AController* Assistant = nullptr;

};

UENUM()
enum class EWeaponUtil : uint8
{
	EWU_OnlyAttack			UMETA(DisplayName = "AttackOnly"),
	EWU_OnlyHeal			UMETA(DisplayName = "OnlyHeal"),
	EWU_BothAttackAndHeal	UMETA(DisplayName = "Attack & Heal")
};

UCLASS()
class TRAININGROOM_API UDamageStruct : public UObject
{
	GENERATED_BODY()
	
};
