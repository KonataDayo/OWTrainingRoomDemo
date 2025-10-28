#include "SkillBase.h"
#include "HeroCharacter.h"
#include "SkillDataAsset.h"
#include "TimerManager.h"
#include "Engine/World.h"

void USkillBase::RefreshSkill()
{
	bCanExecute = true;
	if (SkillData->bStackable) CurrentStacks = SkillData->MaxStack;
}

float USkillBase::GetCooldownTime() const
{
	return SkillData->CooldownTime;
}

ESkillSlot USkillBase::GetSkillType() const
{
	return SkillData->SkillType;
}

void USkillBase::Initialization(USkillDataAsset* SDA)
{
	if (!SDA)
	{
		UE_LOG(LogTemp,Error,TEXT("[%s]Failed to init"),*GetName());
		// Search by name directly
		return;
	}
	SkillData = SDA;
}

void USkillBase::Execute(AActor* Instigator)
{
	UE_LOG(LogTemp,Warning,TEXT("execute"));
	if (!bCanExecute) return;
	if (!SkillData->bStackable) /* Most situations lie here */
	{
		bCanExecute = false;
	}
	else
	{
		CurrentStacks--;
		if (CurrentStacks <= 0)
		{
			bCanExecute = false;
		}
		if (CurrentStacks < 0) CurrentStacks = 0;
	}
	StartCooldown();
}

bool USkillBase::CanExecute() const
{
	return bCanExecute;
}

void USkillBase::StartCooldown()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(Timer);
		World->GetTimerManager().SetTimer(Timer,this,&USkillBase::SetSkillReady, SkillData->CooldownTime,false);
	}
}

void USkillBase::SetSkillReady()
{
	if (!SkillData->bStackable)
	{
		bCanExecute = true;
		UE_LOG(LogTemp,Warning,TEXT("Cooldown Completed"));
	}
	else  // Stackable!!
	{
		bCanExecute = true;
		if (CurrentStacks < SkillData->MaxStack)
		{
			CurrentStacks++;
			StartCooldown();
		}
		else // CurrentStacks == MaxStacks
		{
			UE_LOG(LogTemp,Warning,TEXT("Skill Reaches Max Stacks"));
		}
		UE_LOG(LogTemp,Warning,TEXT("AvailableCount: %d/%d"),CurrentStacks, SkillData->MaxStack);
	}
}
