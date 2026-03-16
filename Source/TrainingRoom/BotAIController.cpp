#include "BotAIController.h"
#include "FSM.h"

void ABotAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!GetPawn()) return;
#if WITH_EDITOR
	UE_LOG(LogTemp,Display,TEXT("[%s] Possess: %s"),*GetName(),*GetPawn()->GetName());
#endif
	// FSM Initialization
	if (FSM_Class) 
	{
		FSM_Instance = NewObject<UFSM>(this, FSM_Class);
		FSM_Instance->Init(this);
	}
}