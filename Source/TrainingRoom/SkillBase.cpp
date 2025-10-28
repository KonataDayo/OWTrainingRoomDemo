#include "SkillBase.h"
#include "HeroCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"

void USkillBase::RefreshSkill()
{
	bCanExecute = true;
	if (bStackable) CurrentStacks = MaxStacks;
}

float USkillBase::GetCooldownTime() const
{
	return CooldownTime;
}

ESkillSlot USkillBase::GetSkillType() const
{
	return SkillType;
}

void USkillBase::Execute(AActor* Instigator)
{
	UE_LOG(LogTemp,Warning,TEXT("execute"));
	if (!bCanExecute) return;
	if (!bStackable) /* Most situations lie here */
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
		World->GetTimerManager().SetTimer(Timer,this,&USkillBase::SetSkillReady,CooldownTime,false);
	}
}

void USkillBase::SetSkillReady()
{
	if (!bStackable)
	{
		bCanExecute = true;
		UE_LOG(LogTemp,Warning,TEXT("Cooldown Completed"));
	}
	else  // Stackable!!
	{
		bCanExecute = true;
		if (CurrentStacks < MaxStacks)
		{
			CurrentStacks++;
			StartCooldown();
		}
		else // CurrentStacks == MaxStacks
		{
			UE_LOG(LogTemp,Warning,TEXT("Skill Reaches Max Stacks"));
		}
		UE_LOG(LogTemp,Warning,TEXT("AvailableCount: %d/%d"),CurrentStacks,MaxStacks);
	}
}
