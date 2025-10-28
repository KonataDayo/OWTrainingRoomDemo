#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StructuralSaveGame.generated.h"

class UInputDefaultDataAsset;
struct FKey;

USTRUCT()
struct FInputSaveDate
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FName, FKey> ActionToKey;

	FInputSaveDate() = default;
	FInputSaveDate(const TMap<FName,FKey>& map)
	{
		ActionToKey = map;
	}
};

UCLASS()
class TRAININGROOM_API UStructuralSaveGame : public USaveGame
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "Input")
	FInputSaveDate InputData;


protected:
	virtual void PostInitProperties() override;

public:
	UStructuralSaveGame();

	const FInputSaveDate& GetInputSaveDate() const;
	void AddInputBinding(FName ActionName, FKey Key);
	void SetInputSaveData(FInputSaveDate Data);
};
