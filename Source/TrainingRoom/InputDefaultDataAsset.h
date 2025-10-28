#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDefaultDataAsset.generated.h"

struct FKey;

UCLASS(BlueprintType)
class TRAININGROOM_API UInputDefaultDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<FName, FKey> ActionToKeyDefault;
public:

	TMap<FName,FKey> GetActionToKeyDefault() const;
};
