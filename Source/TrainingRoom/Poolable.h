// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

class TRAININGROOM_API IPoolable
{
	GENERATED_BODY()

public:
	// Reset States, FX On, Set Lifespan
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category = "ObjectPool")
	void OnActivate();

	// FX Off, Physics Off...
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ObjectPool")
	void OnDeactivate();
};
