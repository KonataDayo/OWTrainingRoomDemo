#pragma once
#include "CoreMinimal.h"
#include "WeaponFireResult.h"
#include "Weapon.generated.h"

/**
 * LINE TRACING IS IMPLEMENTED IN THIS CLASS!
 */
UENUM()
enum class EFireMode : uint8
{
	EFM_Primary		UMETA(DisplayName = "Primary Fire Mode"),
	EFM_Auxiliary	UMETA(DisplayName = "Auxiliary Fire Mode"),
};

USTRUCT()
struct FFireState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EFireMode FireMode;

	UPROPERTY(EditAnywhere)
	float FireInterval = 1.f;

	UPROPERTY(VisibleAnywhere)
	bool bCanFire = true;

	UPROPERTY(EditAnywhere)
	int32 AmmoPerFire = 1;

	UPROPERTY()
	FTimerHandle FireTimer;

	UPROPERTY(VisibleAnywhere)
	bool bWantFire = false;
};

class UDamageSystem;

UCLASS(Blueprintable)
class TRAININGROOM_API UWeapon : public UObject
{
	GENERATED_BODY()
public:

	FORCEINLINE float GetDamage() const;

	FORCEINLINE float GetDecayDistance() const;

	FORCEINLINE float GetDecayRatio() const;

	// Primary fire mode
	virtual FWeaponFireResult PerformFire(AActor* Instigator ,FVector MuzzleLocation, FVector FireDirection);

	// Auxiliary fire mode
	virtual FWeaponFireResult PerformAuxiliaryFire(AActor* Instigator, FVector MuzzleLocation, FVector FireDirection);

	virtual void HandleHit(AActor* Instigator,const FWeaponFireResult& FireResult, UDamageSystem* DamageSystem);

	static float CalculateDistance(AActor* Instigator, AActor* Target);

	void Initialize();

	FORCEINLINE int32 GetMaxAmmo() const;

	FFireState* GetFireState(EFireMode FireMode);

	void ConsumeAmmoForMode(EFireMode FireMode);

	void Reload();

	float GetReloadTime() const;

	bool CanFire(AActor* Instigator, EFireMode FireMode);

	FORCEINLINE int32 GetCurrentAmmo() const;

protected:

	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponAttributes")
	int32 MaxAmmo;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float DecayDistance = 0.f; // 0 means no decay

	UPROPERTY(EditAnywhere)
	float DecayRatio; // (0,1)

	UPROPERTY(EditAnywhere)
	float Range;

	UPROPERTY()
	UDamageSystem* CachedDamageSubsystem;

	UPROPERTY(EditAnywhere)
	TMap<EFireMode, FFireState> FireStateData;

	void ConsumeAmmo(int32 Amount);
};
