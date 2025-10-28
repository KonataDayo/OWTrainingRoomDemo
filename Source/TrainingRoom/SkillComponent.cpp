#include "SkillComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "SkillBase.h"
#include "GameFramework/Actor.h"
#include "HeroCharacter.h"
#include "SkillDataAsset.h"
#include "SkillFactory.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillComponent::TryActivateSkill(AActor* Instigator ,ESkillSlot SkillSlot)
{
	UE_LOG(LogTemp,Warning,TEXT("TryActivate"));
	if (Skills.Contains(SkillSlot))
	{
		if (USkillBase* SkillToActivate = Skills[SkillSlot])
		{
			if (SkillToActivate->CanExecute())
			{
				SkillToActivate->Execute(Instigator);
			}
			else UE_LOG(LogTemp, Warning, TEXT("Skill isn't ready yet"));
		}
	}
}

void USkillComponent::SkillInitialization()
{
	/*
	if (!Primary || !Secondary || !Ultimate) return;
	if (USkillBase* pri = NewObject<USkillBase>(this,Primary)) Skills.Add(ESkillSlot::ESS_Primary,pri);
	if (USkillBase* sec = NewObject<USkillBase>(this,Secondary)) Skills.Add(ESkillSlot::ESS_Secondary,sec);
	if (USkillBase* ult = NewObject<USkillBase>(this,Ultimate)) Skills.Add(ESkillSlot::ESS_Ultimate,ult);
	for (const TPair<ESkillSlot,USkillBase*>& Pair : Skills)
	{
		Pair.Value->RefreshSkill();
	}
	*/
	// TODO: Call Factory Method to create instances of skills to fill up the Map
	if (!SkillFactory) return;
	if (!PrimarySkillData || !SecondarySkillData || !UltimateSkillData)
	{
		UE_LOG(LogTemp,Warning,TEXT("[%s]Skill Data Missing!"),*GetName());
		return;
	}
	if (USkillBase* pri = SkillFactory->CreateSkillInstanceFromData(this,PrimarySkillData)) Skills.Add(ESkillSlot::ESS_Primary,pri);
	if (USkillBase* sec = SkillFactory->CreateSkillInstanceFromData(this, SecondarySkillData)) Skills.Add(ESkillSlot::ESS_Secondary, sec);
	if (USkillBase* ult = SkillFactory->CreateSkillInstanceFromData(this, UltimateSkillData)) Skills.Add(ESkillSlot::ESS_Ultimate, ult);
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!SkillFactory)
	{
		SkillFactory = NewObject<USkillFactory>(this);
	}
	SkillInitialization();
}

void USkillComponent::ProcessProjectileOverlap(AProjectile* Projectile, UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult, const FSkillSpec& Spec)
{
	AActor* InstigatorActor = GetOwner();
	if (!GetWorld() || !InstigatorActor || !Projectile) return;
	Projectile->SetActorEnableCollision(false);
	TSet<AActor*> Targets;

	/*
	switch (Spec.SkillType)
	{
		// line tracing/sweep... logic
		case ESkillTargetType::STT_Single:
		{
			if (OtherActor && OtherActor != InstigatorActor) // Single type logic
			break;
		}
		case ESkillTargetType::STT_AOE:
		{
			FVector Center = OtherActor ? OtherActor->GetActorLocation() : Projectile->GetActorLocation();
			TArray<FOverlapResult> Overlaps;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Projectile);
			bool bAnyOverlap = GetWorld()->OverlapMultiByChannel(
				Overlaps,
				Center,
				FQuat::Identity,
				Spec.CollisionChannel,
				FCollisionShape::MakeSphere(Spec.Radius),
				Params
			);
			if (bAnyOverlap)
			{
				TSet<AActor*> UniqueHits;
				for (const FOverlapResult& R : Overlaps)
				{
					if (AActor* Hit = R.GetActor())
					{
						// Owner should be in, let "ShouldApplyTo" decide everything
						if (UniqueHits.Contains(Hit)) continue;
						UniqueHits.Add(Hit);
						if (ShouldApplyTo(InstigatorActor,Hit,Spec))
						{
							// TODO: Handle Logic HERE, shouldn't this be done somewhere else to make it clear?
							if (HandleAOE(Center,Spec,Hit))
							{
								UE_LOG(LogTemp,Warning,TEXT("Finish Handling AOE Damage"));
							}
						}
					}
				}
			}
			// DEBUG VISUALIZATION
			DrawDebugSphere(
				GetWorld(),
				Center,
				Spec.Radius,
				8,
				FColor::Magenta,
				false,
				5.f
			);
			break;
		}
		case ESkillTargetType::STT_Line:
		{
			break;		
		}
	}
	*/
	if (Spec.SkillLifetimeMode == ESkillLifetimeMode::ESM_Instant)
	{
		FVector HitLocation = SweepResult.bBlockingHit ? SweepResult.GetActor()->GetActorLocation() : OtherActor->GetActorLocation();
		Targets = DetectInstantSkillTargets(InstigatorActor,OtherActor,HitLocation,Spec);
	}
	else // Spec.SkillLifetimeMode == ESkillLifetimeMode::ESM_Persistent
	{
		
	}

	// Destroy Projectile On Time
}

bool USkillComponent::ShouldApplyTo(AActor* Instigator ,AActor* Target, const FSkillSpec& Spec)
{
	AHeroCharacter* TargetHero = Cast<AHeroCharacter>(Target);
	AHeroCharacter* InstigatorHero = Cast<AHeroCharacter>(Instigator);
	if (!TargetHero || !InstigatorHero) return false;
	ESkillTargetAffinity SkillAffinity = Spec.SkillTargetAffinity;
	switch (SkillAffinity)
	{
		case ESkillTargetAffinity::STA_All:
		{
			return true;
		}
		case ESkillTargetAffinity::STA_EnemiesOnly:
		{
			return InstigatorHero->GetTeamType() != TargetHero->GetTeamType();
		}
		case ESkillTargetAffinity::STA_AlliesOnly:
		{
			return InstigatorHero->GetTeamType() == TargetHero->GetTeamType();
		}
		case ESkillTargetAffinity::STA_AllExceptOwner:
		{
			return Target != InstigatorHero;
		}
	}
	return false;
}

TSet<AActor*> USkillComponent::DetectInstantSkillTargets(AActor* Instigator,AActor* HitActor ,FVector HitLocation,
	const FSkillSpec& SkillSpec)
{
	TSet<AActor*> DetectedTargets;
	if (!Instigator || !HitActor) return DetectedTargets;
	switch (SkillSpec.SkillType)
	{
		case ESkillTargetType::STT_Single:
		{
			auto HeroGotHit = Cast<AHeroCharacter>(HitActor);
			if (HeroGotHit && HeroGotHit != Instigator)
			{
				DetectedTargets.Add(HeroGotHit);
			}
			break;
		}
		case ESkillTargetType::STT_AOE:
		{
			FVector Center = HitLocation;
			TArray<FOverlapResult> Overlaps;
			FCollisionQueryParams Params;
			bool bAnyOverlap = GetWorld()->OverlapMultiByChannel(
				Overlaps,
				Center,
				FQuat::Identity,
				SkillSpec.CollisionChannel,
				FCollisionShape::MakeSphere(SkillSpec.Radius),
				Params
			);
			if (bAnyOverlap)
			{
				for (const FOverlapResult& R : Overlaps)
				{
					if (AActor* Hit = R.GetActor())
					{
						// Owner should be in, let "ShouldApplyTo" decide everything
						if (DetectedTargets.Contains(Hit)) continue;
						if (ShouldApplyTo(Instigator, Hit, SkillSpec)) DetectedTargets.Add(Hit);
					}
				}
			}
			// DEBUG VISUALIZATION
			DrawDebugSphere(
				GetWorld(),
				Center,
				SkillSpec.Radius,
				8,
				FColor::Magenta,
				false,
				5.f
			);
			break;
		}
		case ESkillTargetType::STT_Line:
		{
			break;
		}
	}
	return DetectedTargets;
}

void USkillComponent::ProcessDetectedTargets(const TSet<AActor*>& Targets, const FSkillSpec& SkillSpec)
{
	UE_LOG(LogTemp,Warning,TEXT(""));
}



