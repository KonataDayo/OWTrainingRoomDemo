#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.h"
#include "DamageStruct.h"
#include "TeamType.h"
#include "HeroCharacter.generated.h"

class USkillBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroSlain, const FDeathData&, DeathData);

UCLASS()
class TRAININGROOM_API AHeroCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()
protected:
	// Components
	UPROPERTY(VisibleAnywhere)
	class USkillComponent* SkillComponent;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	class UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY()
	class AHeroController* HeroController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWeaponComponent> WeaponComponentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> InfoDisplay;

	UPROPERTY(EditAnywhere)
	ETeam Team;

public:
// wrappers of HealthComponent
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHP() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHP() const;

	UFUNCTION(BlueprintCallable)
	void AlterHP(float Amount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentHP(float hp);

public:
	AHeroCharacter();

	FORCEINLINE ETeam GetTeamType() const;

	UPROPERTY()
	FOnHeroSlain OnHeroSlain;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	USkillComponent* GetSkillComponent() const;

	UFUNCTION(BlueprintCallable)
	UWeaponComponent* GetWeaponComponent() const;

	void RespawningReset();

	virtual void TakeDamage_Implementation(const FDamageData& DamageData) override;

	FORCEINLINE AHeroController* GetHeroController() const;

protected:
	UFUNCTION()
	void OnHeroLanded(const FHitResult& HitResult);
};



