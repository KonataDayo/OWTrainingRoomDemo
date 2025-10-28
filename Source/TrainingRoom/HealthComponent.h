#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGROOM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Hero Attributes")
	float MaxHP;

	UPROPERTY(EditAnywhere, Category = "Hero Attributes")
	float CurrentHP;

public:	

	UHealthComponent();

	void AlterHP(float Amount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentHP(float HP);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHP() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHP() const;

	void ResetHP();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
