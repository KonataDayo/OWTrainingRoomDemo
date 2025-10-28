#pragma once

#include "CoreMinimal.h"
#include "TargetingAbility.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

#define DEBUG_VISUALIZATION

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGROOM_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

	// Range
	UPROPERTY(VisibleAnywhere, Category = "TargetAbility")
	class USphereComponent* TargetingSphere;

	UPROPERTY(EditAnywhere, Category = "TargetAbility")
	float SphereRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "TargetAbility")
	float TickIntervalForTargeting = 0.1f;

	float TimeSinceLastUpdate = 0.f;

	UPROPERTY()
	TWeakObjectPtr<AHeroCharacter> CurrentTarget;

	UPROPERTY()
	AController* OwnerController;

	UPROPERTY()
	TArray<TWeakObjectPtr<AHeroCharacter>> HeroesWithinRange;

	UPROPERTY(EditAnywhere, Category = "TargetAbility")
	ETargetOption TargetOption;

	// presented with cosine!!
	UPROPERTY(EditAnywhere, Category = "TargetAbility")
	float ViewDotThreshold = 0.6f;

public:	
	UTargetingComponent();

protected:
	virtual void BeginPlay() override;

	// Callback functions
	UFUNCTION()
	void CompHandleOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CompHandleEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	UFUNCTION()
	void OnCandidateDestroyed(AActor* DestroyedActor);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE AHeroCharacter* GetCurrentTarget() const;
	FORCEINLINE void SetCurrentTarget(AHeroCharacter* target);
	FORCEINLINE AHeroCharacter* GetCurrentTarget();

	bool ShouldApplyToTarget(AHeroCharacter* Target);


};

