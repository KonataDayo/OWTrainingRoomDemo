#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "TracerBlink.generated.h"

UCLASS()
class TRAININGROOM_API UTracerBlink : public USkillBase
{
	GENERATED_BODY()

protected:

	virtual void Execute(AActor* Instigator) override;
};
