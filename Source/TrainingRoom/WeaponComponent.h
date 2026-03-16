#pragma once
#include "CoreMinimal.h"
#include "DamageStruct.h"
#include "Weapon.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

struct FWeaponFireResult;

UENUM()
enum class EProjectileDamageType : uint8
{
	EPDT_InstantSingle UMETA(DisplayName = "Instant Single"),
	EDPT_PersistentSingle UMETA(DisplayName = "Persistent Single"),
	EDPT_InstantAOE UMETA(DisplayName = "Instant AOE"),
	EDPT_PersistentAOE UMETA(DisplayName = "Persistent AOE")
};

class AController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class TRAININGROOM_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	UWeapon* Weapon;

	UPROPERTY(EditAnywhere,Category="WeaponAttributes")
	EWeaponUtil WeaponUtil;

	UPROPERTY()
	class UDamageSystem* CachedDamageSubsystem;

	UPROPERTY()
	class UObjectPoolSubsystem* CachedObjectPoolSubsystem;

	void ProcessHit(AActor* Instigator,FWeaponFireResult Result);

	void AmmoManage(EFireMode FireMode);

	void OnFireCooldownFinished(AActor* Instigator, EFireMode FireMode);

public:	
	UWeaponComponent();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UWeapon> WeaponClass;

	FTimerHandle ReloadTimer;

	UFUNCTION()
	void HandleProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE EWeaponUtil GetWeaponUtil() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable)
	int32 GetMaxAmmo() const;

	void ChangeFireDesire(AActor* Instigator ,EFireMode FireMode,bool Desire);

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void Fire(AActor* Instigator, EFireMode FireMode);

	UFUNCTION()
	virtual void Fire_R(AActor* Instigator, EFireMode FireMode);
	UFUNCTION()
	virtual void Fire_L(AActor* Instigator, EFireMode FireMode);

	// Instant reload logic
	void Reload();

	float CalculateHitDistance(const AActor* Instigator ,const AActor* Target) const;

	FTransform CalculateCrosshair(AActor* Instigator) const;

	float GetReloadTime() const;
};



