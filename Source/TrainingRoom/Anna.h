#pragma once

#include "CoreMinimal.h"
#include "HeroCharacter.h"
#include "TargetingAbility.h"
#include "Anna.generated.h"

UCLASS()
class TRAININGROOM_API AAnna : public AHeroCharacter, public ITargetingAbility
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UTargetingComponent* TargetingComponent;

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:

	AAnna();

	virtual void ApplyEffectToTarget_Implementation(AHeroCharacter* Target) override;


	UTargetingComponent* GetTargetingComponent() const;
};
