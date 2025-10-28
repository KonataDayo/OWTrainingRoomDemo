#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputMappingComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAction);

class UInputComponent;
// Identifiers to Delegates(actual game logic)
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGROOM_API UInputMappingComponent : public UActorComponent
{
	GENERATED_BODY()

	TMap<FName,FOnAction> ActionDelegates;

public:	
	UInputMappingComponent();

	FOnAction& GetOrCreateActionDelegate(FName ActionName);

	void RegisterBindings(UInputComponent* InputComponent, const EInputEvent& InputEvent, const TArray<FName>& ActionToBind);

	void UnregisterBindings(UInputComponent* InputComponent);

};
