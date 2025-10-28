#include "WeaponComponent.h"
#include "DamageStruct.h"
#include "DamageSystem.h"
#include "Projectile.h"
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
	// check DamageSubsystem
	if (!CachedDamageSubsystem && GetWorld())
	{
		CachedDamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDamageSystem>();
	}
	if (AActor* Instigator = ProjectileActor->GetInstigator())
	{
		// avoid overlapping with ourselves
		if (OtherActor != Instigator)
		{
			float HitDistance = CalculateHitDistance(Instigator,OtherActor);
			FDamageData thisData(Instigator,Weapon,HitDistance);
			CachedDamageSubsystem->ApplyDamage(Instigator,OtherActor,thisData);
		}
	}
	else UE_LOG(LogTemp,Warning,TEXT("[WeaponComponent]%s Failed to call GetOwner()"),*OverlappedComponent->GetName());
}

EWeaponUtil UWeaponComponent::GetWeaponUtil() const
{
	return WeaponUtil;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!Weapon && WeaponClass)
	{
		Weapon = NewObject<UWeapon>(this,WeaponClass);
	}
	if (!CachedDamageSubsystem && GetWorld())
	{
		CachedDamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDamageSystem>();
	}
}

void UWeaponComponent::Fire_R(AActor* Instigator)
{

}

void UWeaponComponent::AmmoManage()
{
	CurrentAmmo--;
	bCanFire = false;
	// reset Fire
	TimeSinceLastFire = 0.f;
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

void UWeaponComponent::UpdateFireSignal(float DeltaTime)
{
	if (bCanFire == true) return;
	if (TimeSinceLastFire < FireInterval) TimeSinceLastFire += DeltaTime;
	else if (TimeSinceLastFire >= FireInterval)
	{
		bCanFire = true;
	}
}

// Encapsulated Algorithm Method
void UWeaponComponent::Fire_L(AActor* Instigator)
{
	if (!CanFire(Instigator)) return;
	AmmoManage();
	FWeaponFireResult FireResult;
	FTransform CrosshairData = CalculateCrosshair(Instigator);
	// Fire function, logic in subordinate classes
	FireResult = Weapon->PerformFire(Instigator, CrosshairData.GetLocation(), CrosshairData.GetRotation().Vector());
	ProcessHit(Instigator,FireResult);
}

void UWeaponComponent::ProcessHit(AActor* Instigator,FWeaponFireResult Result)
{
	if (!Instigator) return;
	switch (Result.WeaponFireType)
	{
		case EWeaponFireType::WFT_Projectile:
		{
			if (auto HB = Result.Projectile->GetHitBox())
			{
				HB->OnComponentBeginOverlap.AddDynamic(this,&UWeaponComponent::HandleProjectileOverlap);
				//HB->OnComponentHit.AddDynamic(this,&UWeaponComponent::HandleProjectileHit);
			}
			break;
		}
		case EWeaponFireType::WFT_Hitscan:
		{
			for (const FHitResult& R : Result.HitResults)
			{
				AActor* Target = R.GetActor();
				float HitDistance = CalculateHitDistance(Instigator,Target);
				FDamageData DamageInfo(Instigator,Weapon,HitDistance);
				CachedDamageSubsystem->ApplyDamage(Instigator,Target,DamageInfo);
				break;
			}
		}
	}
}

void UWeaponComponent::Reload()
{
	CurrentAmmo = MaxAmmo;
	UE_LOG(LogTemp,Warning,TEXT("Reloaded"));
}

float UWeaponComponent::GetReloadTime() const
{
	return ReloadTime;
}

void UWeaponComponent::SetFireState(bool State)
{
	bCanFire = State;
}

bool UWeaponComponent::GetFireState() const
{
	return bCanFire;
}

bool UWeaponComponent::CanFire(const AActor* Instigator) const
{
	return bCanFire && CurrentAmmo > 0 && Instigator && CachedDamageSubsystem && Weapon;
}

float UWeaponComponent::CalculateHitDistance(const AActor* Instigator,const AActor* Target) const
{
	// Negative number means invalid!
	if (!Instigator || !Target) return -1.f;
	return FVector::Dist(Instigator->GetActorLocation(), Target->GetActorLocation());
}

