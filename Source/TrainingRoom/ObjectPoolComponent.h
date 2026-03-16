#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"


class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGROOM_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AProjectile*> ProjectilePool;

	UPROPERTY(EditAnywhere)
	int32 MaxPoolSize = 20;

public:	
	UObjectPoolComponent();

	AProjectile* GetProjectileFromPool(AActor* Instigator, FVector MuzzleLocation, FVector FireDirection);

protected:
	virtual void BeginPlay() override;

};
