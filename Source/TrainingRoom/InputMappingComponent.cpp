#include "InputMappingComponent.h"
#include "Components/InputComponent.h"

UInputMappingComponent::UInputMappingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FOnAction& UInputMappingComponent::GetOrCreateActionDelegate(FName ActionName, const EInputEvent& InputEvent)
{
	FInputContextKey Key;
	Key.InputEvent = InputEvent;
	Key.ActionName = ActionName;
	return ActionDelegates.FindOrAdd(Key);
}

void UInputMappingComponent::RegisterBindings(UInputComponent* InputComponent, const EInputEvent& InputEvent, const TArray<FName>& ActionToBind)
{
	if (!InputComponent) return;
	for (FName ActionName : ActionToBind)
	{
		FInputActionBinding NewBinding(ActionName,InputEvent);
		NewBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this,ActionName,InputEvent]()
		{
			FInputContextKey Key;
			Key.ActionName = ActionName;
			Key.InputEvent = InputEvent;
			if (FOnAction* Del = ActionDelegates.Find(Key))
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
}
