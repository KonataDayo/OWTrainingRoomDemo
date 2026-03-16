#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputMappingComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAction);

USTRUCT()
struct FInputContextKey
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActionName;

	UPROPERTY()
	TEnumAsByte<EInputEvent> InputEvent;

	bool operator==(const FInputContextKey& Other) const
	{
		return ActionName == Other.ActionName && InputEvent == Other.InputEvent;		
	}
	// When Using a FInputContextKey as a KEY of a map, the compiler doesn't know how to calculate a Hash Value for this struct
	friend uint32 GetTypeHash(const FInputContextKey& Key)
	{
		return HashCombine(GetTypeHash(Key.ActionName),GetTypeHash(Key.InputEvent));
	}
};

class UInputComponent;
// Identifiers to Delegates(actual game logic)
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGROOM_API UInputMappingComponent : public UActorComponent
{
	GENERATED_BODY()

	TMap<FInputContextKey,FOnAction> ActionDelegates;

public:	
	UInputMappingComponent();

	FOnAction& GetOrCreateActionDelegate(FName ActionName,const EInputEvent& InputEvent);

	void RegisterBindings(UInputComponent* InputComponent, const EInputEvent& InputEvent, const TArray<FName>& ActionToBind);

	void UnregisterBindings(UInputComponent* InputComponent);

};
