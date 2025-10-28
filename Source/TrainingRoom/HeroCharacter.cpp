#include "HeroCharacter.h"
#include "Display.h"
#include "HealthBarWidgetComponent.h"
#include "HealthComponent.h"
#include "HeroController.h"
#include "SkillBase.h"
#include "SkillComponent.h"
#include "WeaponComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

// wrappers of HealthComponent 
float AHeroCharacter::GetCurrentHP() const
{
	return HealthComponent->GetCurrentHP();
}

float AHeroCharacter::GetMaxHP() const
{
	return HealthComponent->GetMaxHP();
}

void AHeroCharacter::AlterHP(float Amount)
{
	HealthComponent->SetCurrentHP(FMath::Clamp(GetCurrentHP() + Amount, 0.f, GetMaxHP()));
}

void AHeroCharacter::SetCurrentHP(float hp)
{
	HealthComponent->SetCurrentHP(hp);
}
// ends here

AHeroCharacter::AHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	LandedDelegate.AddDynamic(this,&AHeroCharacter::OnHeroLanded);

	// WeaponComponent cannot be spawned right here, since it has to assign the Weapon Class
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetMesh());

}

ETeam AHeroCharacter::GetTeamType() const
{
	return Team;
}

USkillComponent* AHeroCharacter::GetSkillComponent() const
{
	return SkillComponent;	
}

UWeaponComponent* AHeroCharacter::GetWeaponComponent() const
{
	return WeaponComponent;
}

void AHeroCharacter::RespawningReset()
{
	if (!WeaponComponent || !SkillComponent || !HealthComponent) return;
	HealthComponent->ResetHP();
	WeaponComponent->CurrentAmmo = WeaponComponent->MaxAmmo;
	for(auto skill : SkillComponent->Skills)
	{
		auto SkillPtr = skill.Value;
		check(SkillPtr);
		SkillPtr->RefreshSkill();
	}
}

void AHeroCharacter::TakeDamage_Implementation(const FDamageData& DamageData)
{
	IDamageable::TakeDamage_Implementation(DamageData);
	if (!HealthComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("TakeDamage called but HealthComponent is null on %s"),*GetName());
		return;
	}
	AHeroCharacter* InstigatorActor = Cast<AHeroCharacter>(DamageData.Instigator);
	if (!DamageData.Instigator)
	{
		UE_LOG(LogTemp,Warning,TEXT("[AHeroCharacter][TakeDamage INTERFACE]%s failed to cast to AHeroCharacter"),*DamageData.Instigator->GetName());
		return;
	}
	if (!InstigatorActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AHeroCharacter][TakeDamage INTERFACE] damn"));
		return;
	}
	// Determine sign (friendly or enemy)
	int32 Sign = InstigatorActor->GetTeamType() == GetTeamType() ? 1 : -1;
	const float delta =	Sign * DamageData.FinalDamage;
	// Encapsulated Algorithm for calculating damage
	HealthComponent->AlterHP(delta);
	UE_LOG(LogTemp,Warning,TEXT("[Hero %s : %f HP]"),*GetName(),delta);
	if (GetCurrentHP() <= 0.f)
	{
		UE_LOG(LogTemp,Warning,TEXT("Hero %s got slain"),*GetName());
		FDeathData DeathData;
		DeathData.SlainHero = this;
		DeathData.Instigator = InstigatorActor->GetController();
		DeathData.Assistant = nullptr;
		OnHeroSlain.Broadcast(DeathData);
	}
}

AHeroController* AHeroCharacter::GetHeroController() const
{
	return HeroController;
}


void AHeroCharacter::OnHeroLanded(const FHitResult& HitResult)
{
	if (AHeroController* PC = Cast<AHeroController>(GetController()))
	{
		PC->ClearJumpTimes();
	}
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!HeroController) HeroController = Cast<AHeroController>(GetController());
	if (HealthBarWidgetComponent)
	{
		if (auto HealthBar = HealthBarWidgetComponent->GetOrCreateWidget(this))
		{
			if (IsLocallyControlled()) HealthBar->SetVisibility(ESlateVisibility::Hidden);
			else HealthBar->SetVisibility(ESlateVisibility::Visible);
		}
		else UE_LOG(LogTemp,Error,TEXT("Health Bar is NULL"));
	}
	if (WeaponComponentClass && !WeaponComponent)
	{
		WeaponComponent = NewObject<UWeaponComponent>(this,WeaponComponentClass);
		WeaponComponent->RegisterComponent();
	}
	if (InfoDisplay && IsLocallyControlled())
	{
		UUserWidget* UI = CreateWidget<UUserWidget>(GetWorld(), InfoDisplay);
		if (auto DisplayUI = Cast<UDisplay>(UI))
		{
			DisplayUI->OwningHero = this;
			DisplayUI->WeaponComp = WeaponComponent;
			DisplayUI->AddToViewport();
		}
	}
}

void AHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!HeroController)
	{
		HeroController = Cast<AHeroController>(GetController());
		UE_LOG(LogTemp, Warning, TEXT("POST INIT HERO CONTROLLER SET"));
	}
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (WeaponComponent)
	{
		WeaponComponent->UpdateFireSignal(DeltaTime);
	}
} 

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

