#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InputGameInstance.generated.h"

struct FKey;
class UInputDefaultDataAsset;

UCLASS()
class TRAININGROOM_API UInputGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category = "Data")
	UInputDefaultDataAsset* InputDefaultDA;

public:

	TMap<FName,FKey> GetInputDefault();

};
