#include "ObjectPoolComponent.h"
#include "Projectile.h"
#include "Engine/World.h"

UObjectPoolComponent::UObjectPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AProjectile* UObjectPoolComponent::GetProjectileFromPool(AActor* Instigator, FVector MuzzleLocation, FVector FireDirection)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: call Projectile pool"), *Instigator->GetName());
	// TODO: O(n), can use a linked list to optimize it to O(1)
	for (AProjectile* Projectile : ProjectilePool)
	{
		if (!IsValid(Projectile)) continue;
		Projectile->SetOwner(Instigator);
		Projectile->SetInstigator(Cast<APawn>(Instigator));

		Projectile->SetIsSkill(false);

		Projectile->SetActorHiddenInGame(false);
		Projectile->SetActorTickEnabled(true);
		Projectile->SetActorEnableCollision(true);

		Projectile->SetActorLocation(MuzzleLocation);
		Projectile->SetActorRotation(FireDirection.GetSafeNormal().Rotation());
		UE_LOG(LogTemp, Warning, TEXT("%s: %s spawned by pool"), *Instigator->GetName(), *Projectile->GetName());
		return Projectile;
	}
	// no usable projectile in the pool at the moment
	return nullptr;
}


void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	ProjectilePool.Empty();
	UWorld* World = GetWorld();
	AActor* Owner = GetOwner();
	if (!World || !Owner) return;
	for (int32 i = 0; i < MaxPoolSize; i++)
	{
		FActorSpawnParameters Params;
		AProjectile* proj = World->SpawnActor<AProjectile>();
		if (proj)
		{
			proj->SetActorHiddenInGame(true);
			proj->SetActorTickEnabled(false);
			proj->SetActorEnableCollision(false);
			ProjectilePool.Add(proj);
		}
	}
}



