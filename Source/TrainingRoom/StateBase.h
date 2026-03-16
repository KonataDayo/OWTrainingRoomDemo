#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateBase.generated.h"

UENUM()
enum class EStateType : uint8
{
	EST_IDLE			UMETA(DisplayName = "Idle"),
	EST_DEVIATED		UMETA(DisplayName = "Deviated")
};

UCLASS(Abstract ,Blueprintable)
class TRAININGROOM_API UStateBase : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BT;

	UPROPERTY(EditAnywhere)
	EStateType StateType = EStateType::EST_IDLE;

public:

	void Enter(class AAIController* OwnerController);

	void OnExec(AAIController* OwnerController);

	void Exit(AAIController* OwnerController);

	FORCEINLINE EStateType GetStateType() const;
};
