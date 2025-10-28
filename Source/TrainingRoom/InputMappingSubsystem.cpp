#include "InputMappingSubsystem.h"

#include "InputGameInstance.h"
#include "StructuralSaveGame.h"
#include "Engine/World.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

void UInputMappingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UWorld* World = GetWorld();
	if (!World) return;
	UStructuralSaveGame* SG = LoadFromSaveGame();
	UE_LOG(LogTemp,Warning,TEXT("[InputMappingSubsystem]Initialization"));
	RegisterKeyMapping(World,SG);
}

void UInputMappingSubsystem::RegisterKeyMapping(UWorld* World, UStructuralSaveGame* SG)
{
	UInputSettings* InputSettings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!InputSettings || !World || !SG) return;
	for (const TPair<FName, FKey>& Pair : SG->GetInputSaveDate().ActionToKey)
	{
		const FName ActionName = Pair.Key;
		const FKey NewKey = Pair.Value;
		TArray<FInputActionKeyMapping> Current;
		InputSettings->GetActionMappingByName(ActionName,Current);
		for (const FInputActionKeyMapping& Mapping : Current)
		{
			InputSettings->RemoveActionMapping(Mapping,false);
		}
		FInputActionKeyMapping NewMapping(ActionName,NewKey);
		InputSettings->AddActionMapping(NewMapping,false);
	}
	InputSettings->ForceRebuildKeymaps();
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator();It;++It)
	{
		if (APlayerController* PC = It->Get())
		{
			PC->PlayerInput->ForceRebuildingKeyMaps(true);
		}
	}
}

UStructuralSaveGame* UInputMappingSubsystem::LoadFromSaveGame(const FString& SlotName, const int32 UserIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,UserIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("[MappingSub]SG Exists"));
		USaveGame* SG = UGameplayStatics::LoadGameFromSlot(SlotName,UserIndex);
		if (UStructuralSaveGame* ISG = Cast<UStructuralSaveGame>(SG))
		{
			return ISG;
		}
	}
	UStructuralSaveGame* NewSaveGame = Cast<UStructuralSaveGame>(UGameplayStatics::CreateSaveGameObject(UStructuralSaveGame::StaticClass()));
	auto GI = Cast<UInputGameInstance>(GetGameInstance());
	// Initialize when create a new SaveGame slot
	if (NewSaveGame && GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[MappingSub]New SG"));
		FInputSaveDate ISD(GI->GetInputDefault());
		NewSaveGame->SetInputSaveData(ISD);
	}
	return NewSaveGame;
}

bool UInputMappingSubsystem::AddBinding(UWorld* World, FName ActionName, FKey Key, const FString& SlotName,
	const int32 UserIndex)
{
	if (!World) return false;
	UStructuralSaveGame* SG = LoadFromSaveGame(SlotName,UserIndex);
	SG->AddInputBinding(ActionName,Key);
	bool bSaved = SaveInput(SG,SlotName,UserIndex);
	// apply
	RegisterKeyMapping(World,SG);
	return bSaved;
}

bool UInputMappingSubsystem::SaveInput(UStructuralSaveGame* SG, const FString& SlotName, const int32 UserIndex)
{
	if (!SG) return false;
	return UGameplayStatics::SaveGameToSlot(SG,SlotName,UserIndex);
}

