#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "BotAIController.generated.h"

UCLASS()
class TRAININGROOM_API ABotAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFSM> FSM_Class;

	UPROPERTY()
	UFSM* FSM_Instance;

	virtual void OnPossess(APawn* InPawn) override;
};
