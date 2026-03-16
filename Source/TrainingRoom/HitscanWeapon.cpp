#include "HitscanWeapon.h"
#include "WeaponComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "HeroCharacter.h"
#include "DamageSystem.h"

FWeaponFireResult UHitscanWeapon::PerformFire(AActor* Instigator,FVector MuzzleLocation, FVector FireDirection)
{
	// TODO: 1.CONTINUOUS FIRE MISSING? 2.SKILLS FAIL TO EXECUTE(NO RECALL UPDATE INFO)
	Super::PerformFire(Instigator,MuzzleLocation, FireDirection);
	FWeaponFireResult Result;
	if (!Instigator)
	{
		UE_LOG(LogTemp,Warning,TEXT("[Hitscan Weapon] instigator is null"));
		return Result;
	}
	Result.WeaponFireType = EWeaponFireType::WFT_Hitscan;
	FVector EndLocation = MuzzleLocation + FireDirection * Range;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Instigator);
	FHitResult HitscanHitResult;

	GetWorld()->LineTraceSingleByChannel(
		HitscanHitResult,
		MuzzleLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility,
		Params
	);
	if (HitscanHitResult.bBlockingHit)
	{
		Result.HitResults.Add(HitscanHitResult);
	}

#if WITH_EDITOR
	if (HitscanHitResult.bBlockingHit)
	{	
		AActor* HitActor = HitscanHitResult.GetActor();
	}
	DrawDebugLine(
		Instigator->GetWorld(),
		MuzzleLocation,
		EndLocation,
		FColor::Orange,
		false,
		3.0f,
		0,
		1.f
	);
#endif
	return Result;
}

void UHitscanWeapon::HandleHit(AActor* Instigator, const FWeaponFireResult& FireResult, UDamageSystem* DamageSystem)
{
	Super::HandleHit(Instigator, FireResult, DamageSystem);
	for (const FHitResult& R : FireResult.HitResults)
	{
		AActor* Target = R.GetActor();
		float HitDistance = CalculateDistance(Instigator, Target);
		FDamageData DamageInfo(Instigator, this, HitDistance);
		CachedDamageSubsystem->ApplyDamage(Instigator, Target, DamageInfo);
		break;
	}
}
