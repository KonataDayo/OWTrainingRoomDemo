#include "InputGameInstance.h"
#include "InputDefaultDataAsset.h"
#include "GameFramework/PlayerInput.h"

TMap<FName, FKey> UInputGameInstance::GetInputDefault()
{
	if (InputDefaultDA)
	{
		return InputDefaultDA->GetActionToKeyDefault();
	}
	return TMap<FName,FKey>();
}

