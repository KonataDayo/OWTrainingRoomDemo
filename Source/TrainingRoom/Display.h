// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Display.generated.h"

/**
 * 
 */
UCLASS()
class TRAININGROOM_API UDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	class AHeroCharacter* OwningHero;

	UPROPERTY(BlueprintReadOnly)
	class UWeaponComponent* WeaponComp;

protected:
	virtual void NativeConstruct() override;
};
