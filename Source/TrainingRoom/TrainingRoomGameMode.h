// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageStruct.h"
#include "GameFramework/GameMode.h"
#include "TeamType.h"
#include "TrainingRoomGameMode.generated.h"

/**
 * 
 */


UCLASS()
class TRAININGROOM_API ATrainingRoomGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere , Category="Player")
	FVector PlayerSpawnLocation;

	UFUNCTION()
	void HandleHeroSlain(const FDeathData& DeathData);

protected:
	virtual void BeginPlay() override;

public:
	FTimerHandle RespawnTimer;
};
