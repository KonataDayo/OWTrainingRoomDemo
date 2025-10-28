// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingRoomGameMode.h"
#include "EngineUtils.h"
#include "HeroCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"

void ATrainingRoomGameMode::HandleHeroSlain(const FDeathData& DeathData)
{
	DeathData.SlainHero->DisableInput(Cast<APlayerController>(DeathData.SlainHero->GetController()));
	GetWorld()->GetTimerManager().SetTimer(
		RespawnTimer,
		[this,DeathData]()
		{
			// disable input
			DeathData.SlainHero->SetActorLocation(PlayerSpawnLocation);
			UE_LOG(LogTemp,Warning,TEXT("Player %s respawned!"),*(DeathData.SlainHero)->GetName());
			// Respawn logic: recovering HP etc.
			DeathData.SlainHero->RespawningReset();
			DeathData.SlainHero->EnableInput(Cast<APlayerController>(DeathData.SlainHero->GetController()));
		},
		3.f,
		false
	);
}

void ATrainingRoomGameMode::BeginPlay()
{
	Super::BeginPlay();
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();It;++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			PC->GetPawn()->SetActorLocation(PlayerSpawnLocation);
		}
	}

	// event binding of hero getting slain
	for (TActorIterator<AHeroCharacter> It(GetWorld(),AHeroCharacter::StaticClass());It;++It)
	{
		AHeroCharacter* HeroSlain = *It;
		if (HeroSlain)
		{
			HeroSlain->OnHeroSlain.AddDynamic(this,&ATrainingRoomGameMode::HandleHeroSlain);
			UE_LOG(LogTemp,Warning,TEXT("Hero %s death function registered"),*HeroSlain->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hero %s death function registration failed"), *HeroSlain->GetName());
		}
	}

}
