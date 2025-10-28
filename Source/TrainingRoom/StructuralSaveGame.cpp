#include "StructuralSaveGame.h"
#include "GameFramework/PlayerInput.h"

void UStructuralSaveGame::PostInitProperties()
{
	Super::PostInitProperties();
}

UStructuralSaveGame::UStructuralSaveGame()
{

}

const FInputSaveDate& UStructuralSaveGame::GetInputSaveDate() const
{
	return InputData;
}

void UStructuralSaveGame::AddInputBinding(FName ActionName, FKey Key)
{
	// replacement
	if (InputData.ActionToKey.Contains(ActionName))
	{
		InputData.ActionToKey.Remove(ActionName);
	}
	InputData.ActionToKey.Add(ActionName,Key);
}

void UStructuralSaveGame::SetInputSaveData(FInputSaveDate Data)
{
	InputData = Data;
}
