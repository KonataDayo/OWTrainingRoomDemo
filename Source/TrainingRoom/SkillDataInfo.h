#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "UObject/NoExportTypes.h"
#include "SkillDataInfo.generated.h"

// FOR PROJECTILE SKILLS
UENUM()
enum class ESkillTargetType :uint8
{
	STT_Single,
	STT_AOE,
	STT_Cone,
	STT_Line
};

UENUM()
enum class ESkillLifetimeMode
{
	ESM_Instant,
	ESM_Persistent
};

UENUM()
enum class ESkillTargetAffinity : uint8
{
	STA_AlliesOnly,
	STA_EnemiesOnly,
	STA_AllExceptOwner,
	STA_All
};

USTRUCT(BlueprintType)
struct FSkillSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ESkillTargetType SkillType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ESkillLifetimeMode SkillLifetimeMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Radius;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TickInterval;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTargetAffinity SkillTargetAffinity;

};

UCLASS()
class TRAININGROOM_API USkillDataInfo : public UObject
{
	GENERATED_BODY()
	
};
