// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TeamType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETeam : uint8
{
	ET_Neutral UMETA(DisplayName = "Netural"),
	ET_TeamA UMETA(DisplayName = "TeamA"),
	ET_TeamB UMETA(DisplayName = "TeamB")
};

UCLASS()
class TRAININGROOM_API UTeamType : public UObject
{
	GENERATED_BODY()
	
};
