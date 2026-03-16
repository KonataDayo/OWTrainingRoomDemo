#include "WeaponComponent.h"
#include "DamageStruct.h"
#include "DamageSystem.h"
#include "ObjectPoolSubsystem.h"
#include "Projectile.h"
#include "TimerManager.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// ProjectileWeapon Overlap handling logic
void UWeaponComponent::HandleProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OverlappedComponent) return;
	AActor* ProjectileActor = OverlappedComponent->GetOwner();
	if (!ProjectileActor) return;
	// check cached subsystems
	if (!CachedDamageSubsystem && GetWorld()) CachedDamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDamageSystem>();
	if (!CachedObjectPoolSubsystem && GetWorld()) CachedObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();

	if (AActor* Instigator = ProjectileActor->GetInstigator())
	{
		// avoid overlapping with ourselves
		if (OtherActor != Instigator)
		{
			float HitDistance = CalculateHitDistance(Instigator,OtherActor);
			FDamageData thisData(Instigator,Weapon,HitDistance);
			CachedObjectPoolSubsystem->Return(ProjectileActor);
			CachedDamageSubsystem->ApplyDamage(Instigator,OtherActor,thisData);
		}
	}
	else UE_LOG(LogTemp,Warning,TEXT("[WeaponComponent]%s Failed to call GetOwner()"),*OverlappedComponent->GetName());
}

EWeaponUtil UWeaponComponent::GetWeaponUtil() const
{
	return WeaponUtil;
}

int32 UWeaponComponent::GetCurrentAmmo() const
{
	if (!Weapon) return -1;
	return Weapon->GetCurrentAmmo();
}

int32 UWeaponComponent::GetMaxAmmo() const
{
	if (!Weapon) return -1;
	return Weapon->GetMaxAmmo();
}

void UWeaponComponent::ChangeFireDesire(AActor* Instigator, EFireMode FireMode, bool Desire)
{
	if (!Weapon) return;

	FFireState* FireState = Weapon->GetFireState(FireMode);
	if (!FireState)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("[%s] Failed to get fire state for mode %d!"), *GetName(),(int)FireMode);
#endif
		return;
	}
	FireState->bWantFire = Desire;
	// Hit fire button and allow to fire at the moment, then fire immediately
	if (FireState->bWantFire && FireState->bCanFire) Fire(Instigator,FireMode);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!Weapon && WeaponClass)
	{
		Weapon = NewObject<UWeapon>(this,WeaponClass);
		Weapon->Initialize();
	}
	UWorld* World = GetWorld();
	if (!World) return;
	if (!CachedDamageSubsystem && World)
	{
		CachedDamageSubsystem = World->GetGameInstance()->GetSubsystem<UDamageSystem>();
	}
	if (!CachedObjectPoolSubsystem)
	{
		CachedObjectPoolSubsystem = World->GetSubsystem<UObjectPoolSubsystem>();
	}
}

void UWeaponComponent::OnFireCooldownFinished(AActor* Instigator, EFireMode FireMode)
{
	if (!Weapon) return;
	if (FFireState* FireState = Weapon->GetFireState(FireMode))
	{
		FireState->bCanFire = true;
		if (FireState->bWantFire) Fire(Instigator,FireMode);
	}
}

void UWeaponComponent::Fire(AActor* Instigator, EFireMode FireMode)
{
	if (!Instigator || !Weapon) return;
	// Fire Timer
	auto FireState = Weapon->GetFireState(FireMode);
	UWorld* World = GetWorld();
	if (!FireState || !World)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp,Error,TEXT("[%s] Failed to get fire state, please check the initialization function of this weapon/or failed to GetWorld()"),*Weapon->GetName());
#endif
		return;
	}
	// Can fire and wanna fire
	if (!FireState->bCanFire || !FireState->bWantFire)
	{
		UE_LOG(LogTemp,Warning,TEXT("[%s]Cannot Fire or don't wanna fire right now... [STATE]CanFire: %d, WantFire: %d"),*GetName(),FireState->bCanFire,FireState->bWantFire);
		return;
	}

	switch (FireMode)
	{
		case EFireMode::EFM_Primary:
			Fire_L(Instigator, FireMode);
			break;
		case EFireMode::EFM_Auxiliary:
			Fire_R(Instigator, FireMode);
			break;
		default:
#if WITH_EDITOR
			UE_LOG(LogTemp,Warning,TEXT("[%s] Fire Function cannot find FireMode"),*GetName());
#endif
			break;
	}
	// right after Fire function(CanFire statement inside Fire function)
	FireState->bCanFire = false;

	FTimerDelegate FireDelegate;
	FireDelegate.BindUObject(this,&UWeaponComponent::OnFireCooldownFinished,Instigator,FireMode);
	World->GetTimerManager().SetTimer(
		FireState->FireTimer,
		FireDelegate,
		FireState->FireInterval,
		false // Whether loop or not is decided by (func)ResetCanFire
	);
}

void UWeaponComponent::AmmoManage(EFireMode FireMode)
{
	Weapon->ConsumeAmmoForMode(FireMode);
}

FTransform UWeaponComponent::CalculateCrosshair(AActor* Instigator) const
{
	FVector CameraLocation;
	AController* Controller = Instigator->GetInstigatorController();
	if (Controller)
	{
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			FVector CameraDirection;
			int32 x, y;
			PC->GetViewportSize(x, y);
			// Crosshair: center of the screen
			PC->DeprojectScreenPositionToWorld(x*0.5f, y*0.5f, CameraLocation, CameraDirection);
			return FTransform(CameraDirection.Rotation(),CameraLocation);
		}
		FRotator CameraRotation;
		Controller->GetActorEyesViewPoint(CameraLocation, CameraRotation);
		return FTransform(CameraRotation,CameraLocation);
	}
	return FTransform::Identity;
}

// Encapsulated Algorithm Method
void UWeaponComponent::Fire_L(AActor* Instigator, EFireMode FireMode)
{
	if (!Weapon->CanFire(Instigator,FireMode)) return;
	Weapon->ConsumeAmmoForMode(FireMode);
	FWeaponFireResult FireResult;
	FTransform CrosshairData = CalculateCrosshair(Instigator);
	// Fire function, logic in subordinate classes
	FireResult = Weapon->PerformFire(Instigator, CrosshairData.GetLocation(), CrosshairData.GetRotation().Vector());
	ProcessHit(Instigator,FireResult);
}

void UWeaponComponent::Fire_R(AActor* Instigator, EFireMode FireMode)
{
	if (!Weapon->CanFire(Instigator, FireMode)) return;
}

void UWeaponComponent::ProcessHit(AActor* Instigator,FWeaponFireResult Result)
{
	if (!Instigator) return;
	Weapon->HandleHit(Instigator,Result,CachedDamageSubsystem);
}

void UWeaponComponent::Reload()
{
	if (!Weapon) return;
	Weapon->Reload();
	UE_LOG(LogTemp,Warning,TEXT("Reloaded"));
}

float UWeaponComponent::CalculateHitDistance(const AActor* Instigator,const AActor* Target) const
{
	// Negative number means invalid!
	if (!Instigator || !Target) return -1.f;
	return FVector::Dist(Instigator->GetActorLocation(), Target->GetActorLocation());
}

float UWeaponComponent::GetReloadTime() const
{
	if (!Weapon) return -1.f;
	return Weapon->GetReloadTime();
}
