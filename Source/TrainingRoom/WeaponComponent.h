#pragma once
#include "CoreMinimal.h"
#include "DamageStruct.h"
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

	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float FireInterval;

	float TimeSinceLastFire = 0.f;

	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float ReloadTime;

	bool bCanFire = true;

	UPROPERTY()
	UWeapon* Weapon;

	UPROPERTY(EditAnywhere,Category="WeaponAttributes")
	EWeaponUtil WeaponUtil;

	UPROPERTY()
	class UDamageSystem* CachedDamageSubsystem;

	void ProcessHit(AActor* Instigator,FWeaponFireResult Result);

	void AmmoManage();

	
public:	
	UWeaponComponent();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UWeapon> WeaponClass;

	FTimerHandle ReloadTimer;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="WeaponAttributes")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "WeaponAttributes")
	int32 CurrentAmmo;

	UFUNCTION()
	void HandleProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE EWeaponUtil GetWeaponUtil() const;

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	virtual void Fire_R(AActor* Instigator);
	UFUNCTION()
	virtual void Fire_L(AActor* Instigator);

	void Reload();

	FORCEINLINE float GetReloadTime() const;

	FORCEINLINE void SetFireState(bool State);

	FORCEINLINE bool GetFireState() const;

	bool CanFire(const AActor* Instigator) const;

	float CalculateHitDistance(const AActor* Instigator ,const AActor* Target) const;

	FTransform CalculateCrosshair(AActor* Instigator) const;

	void UpdateFireSignal(float DeltaTime);
};



