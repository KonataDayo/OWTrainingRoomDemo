#pragma once
#include "CoreMinimal.h"
#include "SkillDataInfo.h"
#include "Engine/EngineTypes.h"
#include "SkillBase.generated.h"

class USkillFactory;
class USkillDataAsset;
DECLARE_MULTICAST_DELEGATE(FOnCooldownIsOver)

UENUM(BlueprintType)
enum class ESkillSlot : uint8
{
	ESS_Primary UMETA(DisplayName = "PrimarySkill"),
	ESS_Secondary UMETA(DisplayName = "SecondarySkill"),
	ESS_Ultimate UMETA(DisplayName = "UltimateAblitity")
};

UCLASS(Abstract,Blueprintable,BlueprintType)
class TRAININGROOM_API USkillBase : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,Category="Skill")
	virtual void Execute(AActor* Instigator);

	UFUNCTION(BlueprintCallable,Category="Skill")
	virtual bool CanExecute() const;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void StartCooldown();

	FTimerHandle Timer;

	FOnCooldownIsOver OnCooldownIsOver;

	USkillBase() = default;

	void RefreshSkill();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCooldownTime() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ESkillSlot GetSkillType() const;

	void Initialization(USkillDataAsset* SDA);

	FSkillSpec GetSkillSpec() const;

protected:

	UPROPERTY()
	USkillFactory* SkillFactory;

	UPROPERTY()
	USkillDataAsset* SkillData;

	int32 CurrentStacks;

	UPROPERTY()
	bool bCanExecute = true;

	UPROPERTY(EditAnywhere, Category = "SkillProjectileInfo")
	FSkillSpec SkillSpec;
private:
	UFUNCTION()
	void SetSkillReady();

};

