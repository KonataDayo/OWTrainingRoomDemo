
#include "TargetingComponent.h"
#include "HeroCharacter.h"
#include "TargetingAbility.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "HeroController.h"
#include "GameFramework/PlayerController.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (auto Owner = GetOwner())
	{
		if (USceneComponent* Root = Owner->GetRootComponent())
		{
			// constructing object
			TargetingSphere = NewObject<USphereComponent>(Owner,TEXT("TargetingSphere"));
			TargetingSphere->InitSphereRadius(SphereRadius);
			TargetingSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			TargetingSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
			TargetingSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			// registration
			TargetingSphere->RegisterComponent();
			TargetingSphere->AttachToComponent(Root,FAttachmentTransformRules::KeepRelativeTransform);
			// Callback functions
			TargetingSphere->OnComponentBeginOverlap.AddDynamic(this,&UTargetingComponent::CompHandleOverlap);
			TargetingSphere->OnComponentEndOverlap.AddDynamic(this,&UTargetingComponent::CompHandleEndOverlap);
		}
	}
	// controller init
	OwnerController = GetOwner()->GetInstigatorController();
	if (!OwnerController)
	{
		if (ACharacter* charOwner = Cast<ACharacter>(GetOwner()))
		{
			OwnerController = charOwner->GetController();
		}
	}
}

void UTargetingComponent::CompHandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp,Warning,TEXT("[TargetingComponent]Owner is null!!!!"));
		return;
	}
	AHeroCharacter* OwnerHero = Cast<AHeroCharacter>(GetOwner());
	AHeroCharacter* OverlapHero = Cast<AHeroCharacter>(OtherActor);
	// Maintaining Candidates
	if (OwnerHero && OverlapHero)
	{
		if (!HeroesWithinRange.Contains(OverlapHero))
		{
			HeroesWithinRange.Add(OverlapHero);
			// Destroy logic
			OverlapHero->OnDestroyed.AddDynamic(this,&UTargetingComponent::OnCandidateDestroyed);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("no interface implemented"));
		}
	}
}

// Also for maintaining candidates
void UTargetingComponent::CompHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AHeroCharacter* ExitHero = Cast<AHeroCharacter>(OtherActor);
	if (ExitHero)
	{
		HeroesWithinRange.Remove(TWeakObjectPtr<AHeroCharacter>(ExitHero));
		ExitHero->OnDestroyed.RemoveDynamic(this,&UTargetingComponent::OnCandidateDestroyed);
		if (CurrentTarget.Get() == ExitHero) CurrentTarget = nullptr;
	}
}

// Heroes in range but got slain
void UTargetingComponent::OnCandidateDestroyed(AActor* DestroyedActor)
{
	if (!DestroyedActor) return;
	AHeroCharacter* H = Cast<AHeroCharacter>(DestroyedActor);
	if (H) HeroesWithinRange.Remove(TWeakObjectPtr<AHeroCharacter>(H));
	if (CurrentTarget.Get() == H) CurrentTarget = nullptr;
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!TargetingSphere)
	{
		UE_LOG(LogTemp, Warning, TEXT("Targeting Sphere null"));
		return;
	}
	if (!OwnerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("owner controller null"));
		return;
	}

	// Tick Interval control
	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate < TickIntervalForTargeting) return;
	TimeSinceLastUpdate = 0.f;

#ifdef DEBUG_VISUALIZATION
	DrawDebugSphere(
		GetWorld(),
		TargetingSphere->GetComponentLocation(),
		SphereRadius,
		10,
		FColor::Orange,
		false,
		1.f
	);
#endif

	AHeroCharacter* BestTarget = nullptr;
	if (HeroesWithinRange.Num() != 0)
	{
		// if all candidates are out of sight, then BestTarget is nullptr automatically
		float BestAngelCosine = -1.f;
		FVector PlayerLocation;
		FRotator PlayerViewRotation;
		if (OwnerController)
		{
			OwnerController->GetPlayerViewPoint(PlayerLocation,PlayerViewRotation);
		}
		else if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->GetPlayerViewPoint(PlayerLocation,PlayerViewRotation);
		}
		else
		{
			PlayerLocation = GetOwner()->GetActorLocation();
			PlayerViewRotation = GetOwner()->GetActorRotation();
		}
		FVector PlayerViewDir = PlayerViewRotation.Vector().GetSafeNormal();
		for (auto CandidateWeak : HeroesWithinRange)
		{
			AHeroCharacter* CandidateHero = CandidateWeak.Get();
			if (!CandidateHero || !ShouldApplyToTarget(CandidateHero)) continue;
			// check if blocked
			if (OwnerController && !OwnerController->LineOfSightTo(CandidateHero)) continue;
			FVector CandidateLocation = CandidateHero->GetActorLocation();
			FVector CandidateToPlayer = (CandidateLocation - PlayerLocation).GetSafeNormal();
			float DotProd = FVector::DotProduct(PlayerViewDir,CandidateToPlayer);
			if (DotProd < ViewDotThreshold) continue;
			// update
			if (DotProd > BestAngelCosine)
			{
				BestTarget = CandidateHero;
				BestAngelCosine = DotProd;
			}
		}
		// Best Target Result is out now
	}
	if (BestTarget != CurrentTarget.Get())
	{
		CurrentTarget = TWeakObjectPtr<AHeroCharacter>(BestTarget);

#ifdef DEBUG_VISUALIZATION
		if (BestTarget)
		{
			DrawDebugSphere(
				GetWorld(),
				BestTarget->GetActorLocation(),
				15.f,
				8,
				FColor::Cyan,
				false,
				0.5f
			);
		}
#endif
	}
}

AHeroCharacter* UTargetingComponent::GetCurrentTarget() const
{
	return CurrentTarget.Get();
}

void UTargetingComponent::SetCurrentTarget(AHeroCharacter* target)
{
	if (CurrentTarget.Get() == target) return;
	CurrentTarget = TWeakObjectPtr<AHeroCharacter>(target);
}

AHeroCharacter* UTargetingComponent::GetCurrentTarget()
{
	return CurrentTarget.Get();
}

bool UTargetingComponent::ShouldApplyToTarget(AHeroCharacter* Target)
{
	if (AHeroCharacter* OwnerHero = Cast<AHeroCharacter>(GetOwner()))
	{
		ETeam OwnerTeam = OwnerHero->GetTeamType();
		ETeam TargetTeam = Target->GetTeamType();
		switch (TargetOption)
		{
			case ETargetOption::ETO_OnlyAllies:
			{
				return OwnerTeam == TargetTeam;
			}
			case ETargetOption::ETO_OnlyEnemies:
			{
				return OwnerTeam != TargetTeam; 
			}
			case ETargetOption::ETO_AllHeroes:
			{
				return true;		
			}
		}
	}
	UE_LOG(LogTemp,Warning,TEXT("[TargetingComp]Target Cast failed"));
	return false;
}
