#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void UHealthComponent::AlterHP(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.f, MaxHP);
}

void UHealthComponent::SetCurrentHP(float HP)
{
	CurrentHP = HP;
}

float UHealthComponent::GetCurrentHP() const
{
	return CurrentHP;
}

float UHealthComponent::GetMaxHP() const
{
	return MaxHP;
}

void UHealthComponent::ResetHP()
{
	CurrentHP = MaxHP;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

