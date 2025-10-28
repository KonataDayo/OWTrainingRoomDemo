// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "TracerRecall.generated.h"

/**
 * 
 */

USTRUCT()
struct FRecallInfo
{
	GENERATED_BODY()

	FVector Location;

	FRotator Rotation;

	float HP;

};

UCLASS()
class TRAININGROOM_API UTracerRecall : public USkillBase
{
	GENERATED_BODY()

	UPROPERTY()
	FRotator RotationThreeSecAgo;

	UPROPERTY()
	FVector LocationThreeSecAgo;

	void Execute(AActor* Instigator) override;

	UPROPERTY()
	TArray<FRecallInfo> RecallInfos;

public:
	void SetLocationAndRotation(FVector Location,FRotator Rotation);

	friend class ATracer;
};
