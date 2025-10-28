#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "AnnaNanoBoost.generated.h"

UCLASS()
class TRAININGROOM_API UAnnaNanoBoost : public USkillBase
{
	GENERATED_BODY()
protected:
	virtual void Execute(AActor* Instigator) override;
};
