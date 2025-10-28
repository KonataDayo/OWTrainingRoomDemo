#include "BotHero.h"
#include "TimerManager.h"
#include "WeaponComponent.h"
#include "Engine/World.h"

void ABotHero::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponComponent);
	TimerDel.BindUFunction(GetWeaponComponent(),FName("Fire_L"),this);
	FireRegularly();
}

void ABotHero::FireRegularly()
{
	GetWorld()->GetTimerManager().SetTimer(
		RegularFireTimer,
		TimerDel,
		RegularFireInterval,
		true
	);
}
