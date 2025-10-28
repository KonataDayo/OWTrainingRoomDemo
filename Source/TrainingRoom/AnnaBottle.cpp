#include "AnnaBottle.h"
#include "Anna.h"
#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "SkillComponent.h"

void UAnnaBottle::Execute(AActor* Instigator)
{
	Super::Execute(Instigator);
	if (!BottleClass || !Instigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bottle Class or Instigator NULL"));
		return;
	}
	UWorld* World = Instigator->GetWorld();
	AController* HeroController = Instigator->GetInstigatorController();
	if (!World || !HeroController) return;

	// Spawning Projectile
	FVector StartLocation;
	FRotator ThrowRotation;
	HeroController->GetPlayerViewPoint(StartLocation,ThrowRotation);
	FActorSpawnParameters Params;
	Params.Instigator = Cast<APawn>(Instigator);
	AProjectile* Bottle = World->SpawnActor<AProjectile>(BottleClass,StartLocation + Instigator->GetActorForwardVector()*15.f,ThrowRotation,Params);
	if (Bottle)
	{
		UE_LOG(LogTemp,Warning,TEXT("Bottle Spawned"));
		Bottle->SetOwner(Instigator);
		Bottle->SetIsSkill(true);
		// effect call
		// TODO: Stop spawning projectiles manually, should use a factory method to make our code tidy and neat!!
		FSkillSpec Spec;
		Spec.SkillType = ESkillTargetType::STT_AOE;
		Spec.SkillLifetimeMode = ESkillLifetimeMode::ESM_Instant;
		Spec.Radius = 300.f;
		Spec.Damage = 50.f;
		Bottle->InitSkill(Spec);
		if (USkillComponent* SkillComp = Instigator->FindComponentByClass<USkillComponent>())
		{
			Bottle->OnSkillProjectileActivate.AddUObject(SkillComp,&USkillComponent::ProcessProjectileOverlap);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Bottle Didn't Spawned"));
	}
}
