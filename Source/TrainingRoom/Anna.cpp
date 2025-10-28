#include "Anna.h"
#include "DrawDebugHelpers.h"
#include "TargetingComponent.h"
#include "Components/SphereComponent.h"


void AAnna::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AAnna::BeginPlay()
{
	Super::BeginPlay();
}

void AAnna::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!TargetingComponent)
	{
		TargetingComponent = NewObject<UTargetingComponent>(this,UTargetingComponent::StaticClass());
		TargetingComponent->RegisterComponent();
		UE_LOG(LogTemp, Warning, TEXT("[%s]Targeting comp fixed in PostInitialization"),*GetName());
	}
}


AAnna::AAnna()
{
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("TargetingComponent"));
}

void AAnna::ApplyEffectToTarget_Implementation(AHeroCharacter* Target)
{
	// apply nano boost to target
	if (!Target) return;
	ITargetingAbility::ApplyEffectToTarget_Implementation(Target);
	// TODO: should not be executed if target is null!
	Target->AlterHP(Target->GetMaxHP() * 0.5f);
	UE_LOG(LogTemp,Warning,TEXT("You're powered up, %s"),*Target->GetName());

}

UTargetingComponent* AAnna::GetTargetingComponent() const
{
	return TargetingComponent;
}
