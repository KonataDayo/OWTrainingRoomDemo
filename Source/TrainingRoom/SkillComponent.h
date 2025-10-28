#pragma once

#include "CoreMinimal.h"
#include "SkillDataInfo.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class USkillFactory;
enum class ESkillSlot : uint8;
class AProjectile;
class USkillBase;

DECLARE_MULTICAST_DELEGATE_EightParams(FOnProjectileOverlap, AProjectile*, UPrimitiveComponent*,
	AActor*, UPrimitiveComponent*, int32, bool,const FHitResult&, const FSkillSpec&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGROOM_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	USkillFactory* SkillFactory;

public:	

	//UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SkillSlot")
	//TSubclassOf<USkillBase> Primary;
	//UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "SkillSlot")
	//TSubclassOf<USkillBase> Secondary;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "SkillSlot")
	//TSubclassOf<USkillBase> Ultimate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillSlot")
	TSubclassOf<class USkillDataAsset> PrimarySkillData; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillSlot")
	TSubclassOf<USkillDataAsset> SecondarySkillData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillSlot")
	TSubclassOf<USkillDataAsset> UltimateSkillData;

	// Sets default values for this component's properties
	USkillComponent();

	void TryActivateSkill(AActor* Instigator,ESkillSlot SkillSlot);

	UPROPERTY(VisibleAnywhere , Category = "SkillSlot")
	TMap<ESkillSlot,USkillBase*> Skills;

	// Skill Process Functions
	void ProcessProjectileOverlap(AProjectile* Projectile,
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult,
		const FSkillSpec& Spec);

	bool ShouldApplyTo(AActor* Instigator ,AActor* Target,const FSkillSpec& Spec);

	// TODO
	TSet<AActor*> DetectInstantSkillTargets(AActor* Instigator, AActor* HitActor,FVector HitLocation , const FSkillSpec& SkillSpec);

	void ProcessDetectedTargets(const TSet<AActor*>& Targets, const FSkillSpec& SkillSpec);

	void SkillInitialization();
protected:
	// Called when the game starts
	virtual void BeginPlay() override; 
};
