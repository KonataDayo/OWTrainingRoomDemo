// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageStruct.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI,BlueprintType)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class TRAININGROOM_API IDamageable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void TakeDamage(const FDamageData& DamageData);
};
