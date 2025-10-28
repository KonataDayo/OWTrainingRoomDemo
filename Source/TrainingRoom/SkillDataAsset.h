#pragma once

#include "CoreMinimal.h"
#include "SkillDataInfo.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.generated.h"

enum class ESkillSlot : uint8;

UCLASS(Blueprintable,BlueprintType)
class TRAININGROOM_API USkillDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillSlot SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USkillBase> Skill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillSpec SkillSpec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bStackable"))
	int32 MaxStack = 1;

};
