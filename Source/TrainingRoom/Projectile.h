#pragma once

#include "CoreMinimal.h"
#include "Poolable.h"
#include "SkillDataInfo.h"
#include "WeaponComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

DECLARE_MULTICAST_DELEGATE_EightParams(FOnSkillProjectileActivate, AProjectile*, UPrimitiveComponent*, AActor*,
                                       UPrimitiveComponent*, int32, bool, const FHitResult&, const FSkillSpec&);

UCLASS()
class TRAININGROOM_API AProjectile : public AActor, public IPoolable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	class UBoxComponent* ProjectileHitBox;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	USkeletalMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	EProjectileDamageType DamageType;

	bool bInUse = false;

	UPROPERTY()
	UWeapon* OwningWeapon;

	UPROPERTY()
	FTimerHandle ExpireTimer;

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	float ExpireTime = 5.f;

public:	
	AProjectile();

	FOnSkillProjectileActivate OnSkillProjectileActivate;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Attributes")
	bool bIsSkill = false;

	FSkillSpec SkillSpec;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExpireHandler();

public:	
	virtual void Tick(float DeltaTime) override;

	void InitSkill(const FSkillSpec& InSpec);

	FORCEINLINE UBoxComponent* GetHitBox() const;

	FORCEINLINE EProjectileDamageType GetDamageType() const;

	void SetIsSkill(bool IsSkill);

	void SetOwningWeapon(UWeapon* Weapon);

	void OnActivate_Implementation() override;

	void OnDeactivate_Implementation() override;

	void InitializeVelocity(FVector FireDirection);
};

