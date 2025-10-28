#include "SkillFactory.h"
#include "SkillBase.h"
#include "SkillDataAsset.h"

USkillBase* USkillFactory::CreateSkillInstanceFromData(UObject* Outer, TSubclassOf<USkillDataAsset> SkillData)
{
	if (!Outer || !SkillData)
	{
		UE_LOG(LogTemp,Error,TEXT("[SkillFactory]Failed to create skill instance"));
		return nullptr;
	}
	if (auto SkillDataCDO = SkillData.GetDefaultObject())
	{
		FSkillSpec SkillSpec = SkillDataCDO->SkillSpec;
		auto Skill = SkillDataCDO->Skill;
		if (Skill)
		{
			if (USkillBase* SkillCreated = NewObject<USkillBase>(Outer,Skill))
			{
				SkillCreated->Initialization(SkillDataCDO);
				SkillCreated->RefreshSkill();
				return SkillCreated;
			}
		}
	}
	return nullptr;
}
