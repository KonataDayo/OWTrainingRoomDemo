#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillFactory.generated.h"

class USkillDataAsset;
class USkillBase;

UCLASS()
class TRAININGROOM_API USkillFactory : public UObject
{
	GENERATED_BODY()

public:
	USkillBase* CreateSkillInstanceFromData(UObject* Outer, TSubclassOf<USkillDataAsset> SkillData);
};
