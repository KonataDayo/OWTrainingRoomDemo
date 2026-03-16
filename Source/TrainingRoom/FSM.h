#pragma once
#include "CoreMinimal.h"
#include "StateBase.h"
#include "FSM.generated.h"

UCLASS(Blueprintable)
class TRAININGROOM_API UFSM : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	UStateBase* CurrentState;

	UPROPERTY()
	TMap<EStateType, UStateBase*> States;

	UPROPERTY(EditAnywhere)
	TMap<EStateType ,TSubclassOf<UStateBase>> StateDefinitions;

	UPROPERTY()
	TWeakObjectPtr<AAIController> OwnerAIController;

public:
	void Init(AAIController* OwnerController);

	void ChangeState(EStateType NewState);
};
