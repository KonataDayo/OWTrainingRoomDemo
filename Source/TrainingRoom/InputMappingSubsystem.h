#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputMappingSubsystem.generated.h"

class UStructuralSaveGame;
struct FKey;

UCLASS()
class TRAININGROOM_API UInputMappingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

 	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	void RegisterKeyMapping(UWorld* World, UStructuralSaveGame* SG);

	UStructuralSaveGame* LoadFromSaveGame(const FString& SlotName = TEXT("PlayerInputSG"), const int32 UserIndex = 0);

	bool AddBinding(UWorld* World, FName ActionName, FKey Key, const FString& SlotName = TEXT("PlayerInputSG"), const int32 UserIndex = 0);

	bool SaveInput(UStructuralSaveGame* SG, const FString& SlotName = TEXT("PlayerInputSG"), const int32 UserIndex = 0);
};
