#include "FSM.h"
#include "StateBase.h"
#include "Templates/SubclassOf.h"

void UFSM::Init(AAIController* OwnerController)
{
	if (!OwnerController || StateDefinitions.Num() == 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("[%s] OwnerController Null/ States hasn't been specified through BP"),*GetName());
		return;
	}
	States.Empty();
	for (const TPair<EStateType,TSubclassOf<UStateBase>>& StateDef : StateDefinitions)
	{
		auto TempState = NewObject<UStateBase>(this,StateDef.Value);
		if (!TempState) continue;
		EStateType StateTypeTag = TempState->GetStateType();
		States.Add(StateTypeTag, TempState);
		UE_LOG(LogTemp,Display,TEXT("[%s] %s has been registered"),*GetName(),*TempState->GetName());
	}
	if (States.Contains(EStateType::EST_IDLE)) CurrentState = States[EStateType::EST_IDLE];
	else UE_LOG(LogTemp,Error,TEXT("[%s] Failed to assign initial state"),*GetName());
}

void UFSM::ChangeState(EStateType NewState)
{
	if (CurrentState->GetStateType() == NewState || !States.Find(NewState) || !CurrentState) return;
	auto AIC = OwnerAIController.Get();
	if (!AIC)
	{
		UE_LOG(LogTemp,Error,TEXT("[%s] Owner AI Controller NULL"),*GetName());
		return;
	}
	CurrentState->Exit(AIC);
	auto TempNewState = States[NewState];
	if (!TempNewState)
	{
		UE_LOG(LogTemp,Error,TEXT("[%s] Failed to initialize/Cannot find State %d"),*GetName(),NewState);
		if (!StateDefinitions.Contains(NewState)) return;
		TempNewState = NewObject<UStateBase>(this,StateDefinitions[NewState]);
		States.Add(NewState, TempNewState);
	}
	CurrentState = TempNewState;
	CurrentState->Enter(AIC);
}
