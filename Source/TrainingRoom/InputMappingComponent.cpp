#include "InputMappingComponent.h"
#include "Components/InputComponent.h"

UInputMappingComponent::UInputMappingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FOnAction& UInputMappingComponent::GetOrCreateActionDelegate(FName ActionName)
{
	return ActionDelegates.FindOrAdd(ActionName);
}

void UInputMappingComponent::RegisterBindings(UInputComponent* InputComponent, const EInputEvent& InputEvent, const TArray<FName>& ActionToBind)
{
	if (!InputComponent) return;
	for (FName ActionName : ActionToBind)
	{
		FInputActionBinding NewBinding(ActionName,InputEvent);
		NewBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this,ActionName]()
		{
			if (FOnAction* Del = ActionDelegates.Find(ActionName))
			{
				Del->Broadcast();
			}
		}		
		);
		InputComponent->AddActionBinding(NewBinding);
	}
}

void UInputMappingComponent::UnregisterBindings(UInputComponent* InputComponent)
{
	if (!InputComponent) return;
	for (TPair<FName,FOnAction>& Pair : ActionDelegates)
	{
		//Pair.Value.RemoveAll();
	}
}
