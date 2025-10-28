// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "AnnaBottle.generated.h"

/**
 * 
 */
UCLASS()
class TRAININGROOM_API UAnnaBottle : public USkillBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> BottleClass;

protected:
	virtual void Execute(AActor* Instigator) override;
};
