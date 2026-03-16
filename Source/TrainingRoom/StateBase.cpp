#include "StateBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"

void UStateBase::Enter(class AAIController* OwnerController)
{
	if (!OwnerController || !BT)
	{
		UE_LOG(LogTemp,Warning,TEXT("[%s] AI_Controller/BehaviorTree Missing"),*GetName());
		return;
	}
	OwnerController->RunBehaviorTree(BT);
#if	WITH_EDITOR
	UE_LOG(LogTemp,Display,TEXT("[%s] Running BT: %s"),*GetName(),*BT->GetName());
#endif
}

void UStateBase::OnExec(AAIController* OwnerController)
{

}

void UStateBase::Exit(AAIController* OwnerController)
{
#if WITH_EDITOR
	UE_LOG(LogTemp,Display,TEXT("[%s] Exit"),*GetName());
#endif
	if (!OwnerController || !BT)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] AI_Controller/BehaviorTree Missing"), *GetName());
		return;
	}
	if (auto BTComp = OwnerController->FindComponentByClass<UBehaviorTreeComponent>())
	{
		BTComp->StopTree();
	}
}

EStateType UStateBase::GetStateType() const
{
	return StateType;
}
