#include "Weapon.h"
#include "GameFramework/Actor.h"

float UWeapon::GetDamage() const
{
	return Damage;
}

float UWeapon::GetDecayDistance() const
{
	return DecayDistance;
}

float UWeapon::GetDecayRatio() const
{
	return DecayRatio;
}

FWeaponFireResult UWeapon::PerformFire(AActor* Instigator,FVector MuzzleLocation, FVector FireDirection)
{
	FWeaponFireResult f;
	/*
	if (!FireStrategy)
	{
		UE_LOG(LogTemp,Warning,TEXT("[%s] FireStrategy Missing!"),*GetName());
		return f;
	}
	f = FireStrategy->ExecuteFire(Instigator,this,MuzzleLocation,FireDirection);
	*/
	return f;
}

FWeaponFireResult UWeapon::PerformAuxiliaryFire(AActor* Instigator, FVector MuzzleLocation, FVector FireDirection)
{
	FWeaponFireResult f;
	return f;
}

void UWeapon::HandleHit(AActor* Instigator, const FWeaponFireResult& FireResult, UDamageSystem* DamageSystem)
{
}

float UWeapon::CalculateDistance(AActor* Instigator, AActor* Target)
{
	if (!Instigator || !Target)
	{
		UE_LOG(LogTemp,Error,TEXT("[Weapon] (FUNC)CalculateDistance: Instigator or Target is NULLPTR"));
		return -2;
	}
	return FVector::Dist(Instigator->GetActorLocation(), Target->GetActorLocation());
}

void UWeapon::Initialize()
{
	if (!FireStateData.Contains(EFireMode::EFM_Primary))
	{
		FFireState PriFireState;
		PriFireState.FireMode = EFireMode::EFM_Primary;
		PriFireState.AmmoPerFire = -1;
		PriFireState.FireInterval = -1;
		PriFireState.bCanFire = true;
		PriFireState.bWantFire = false;
		FireStateData.Add(EFireMode::EFM_Primary,PriFireState);
		UE_LOG(LogTemp,Error,TEXT("[%s] Cannot find EFM_Primary Fire Mode, Please check the BP settings"),*GetName());
		return;
	}
	if (!FireStateData.Contains(EFireMode::EFM_Auxiliary))
	{
		FFireState AuxFireState;
		AuxFireState.FireMode = EFireMode::EFM_Auxiliary;
		AuxFireState.AmmoPerFire = -1;
		AuxFireState.FireInterval = -1;
		AuxFireState.bCanFire = true;
		AuxFireState.bWantFire = false;
		FireStateData.Add(EFireMode::EFM_Auxiliary,AuxFireState);
		UE_LOG(LogTemp, Error, TEXT("[%s] Cannot find EFM_Auxiliary Fire Mode, Please check the BP settings"), *GetName());
		return;
	}
	CurrentAmmo = MaxAmmo;
	for (TPair<EFireMode, FFireState>& Pair : FireStateData)
	{
		auto& State = Pair.Value;
		State.bCanFire = true;
	}
}

int32 UWeapon::GetMaxAmmo() const
{
	return MaxAmmo;
}

void UWeapon::ConsumeAmmo(int32 Amount)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo - Amount,0,MaxAmmo);
}
 
FFireState* UWeapon::GetFireState(EFireMode FireMode)
{
	if (!FireStateData.Contains(FireMode))
	{
		UE_LOG(LogTemp,Error,TEXT("[%s] Failed to get fire state"),*GetName());
		return nullptr;
	}
	return &FireStateData[FireMode];
}

void UWeapon::ConsumeAmmoForMode(EFireMode FireMode)
{
	if (auto State = FireStateData.Find(FireMode)) ConsumeAmmo(State->AmmoPerFire);
}

void UWeapon::Reload()
{
	CurrentAmmo = MaxAmmo;
}

float UWeapon::GetReloadTime() const
{
	return ReloadTime;
}

bool UWeapon::CanFire(AActor* Instigator, EFireMode FireMode)
{
	if (CurrentAmmo <= 0) return false;
	if (!FireStateData.Contains(FireMode)) return false;
	return FireStateData[FireMode].bCanFire;
}

int32 UWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}



