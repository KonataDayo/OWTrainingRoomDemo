// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrosshairHUD.generated.h"

/**
 * 
 */
UCLASS()
class TRAININGROOM_API ACrosshairHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairTexture;

protected:
	virtual void DrawHUD() override;

};
