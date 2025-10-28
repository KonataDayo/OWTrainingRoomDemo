#include "HeroController.h"
#include "HeroCharacter.h"
#include "InputMappingComponent.h"
#include "InputMappingSubsystem.h"
#include "SkillBase.h"
#include "SkillComponent.h"
#include "TimerManager.h"
#include "WeaponComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

void AHeroController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!CachedMappingSubsystem) CachedMappingSubsystem = GetGameInstance()->GetSubsystem<UInputMappingSubsystem>();
	// this is pretty fucked up lol
	CachedMappingSubsystem->SaveInput(CachedMappingSubsystem->LoadFromSaveGame());
	InputMappingComponent->UnregisterBindings(InputComponent);
}

AHeroController::AHeroController()
{
	InputMappingComponent = CreateDefaultSubobject<UInputMappingComponent>(TEXT("InputMappingComponent"));
}

void AHeroController::RebindPrimaryToQ_ForTestPurpose()
{
	FName Action = FName("PrimarySkill");
	FKey Key = EKeys::Q;
	if (!CachedMappingSubsystem) return;
	bool bRebind = CachedMappingSubsystem->AddBinding(GetWorld(),Action,Key,TEXT("PlayerInputSG"),0);
	if (bRebind)
	{
		UE_LOG(LogTemp,Warning,TEXT("[HeroController] Rebind Succeeded"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[HeroController] Rebind Failed"));
	}
}

void AHeroController::OnPossess(APawn* InPawn)
{
	APlayerController::OnPossess(InPawn);
}

void AHeroController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!CachedMappingSubsystem && GetWorld())
	{
		CachedMappingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInputMappingSubsystem>();
	}
	if (InputComponent)
	{
		if (InputMappingComponent)
		{
			TArray<FName> PressActions = {FName("FireLeft"), FName("Jump"), FName("PrimarySkill"), FName("SecondarySkill"), FName("Reload")};
			InputMappingComponent->RegisterBindings(InputComponent,IE_Pressed,PressActions);
			InputMappingComponent->GetOrCreateActionDelegate(FName("FireLeft")).AddUObject(this,&AHeroController::TryFire_L);
			InputMappingComponent->GetOrCreateActionDelegate(FName("Jump")).AddUObject(this,&AHeroController::Jump);
			InputMappingComponent->GetOrCreateActionDelegate(FName("PrimarySkill")).AddUObject(this,&AHeroController::TryActivatePrimary_Controller);
			InputMappingComponent->GetOrCreateActionDelegate(FName("SecondarySkill")).AddUObject(this, &AHeroController::TryActivateSecondary_Controller);
			InputMappingComponent->GetOrCreateActionDelegate(FName("Reload")).AddUObject(this,&AHeroController::ReloadCall);
			TArray<FName> ReleaseAction = {FName("FireLeft")};
			InputMappingComponent->RegisterBindings(InputComponent,IE_Released,ReleaseAction);
			InputMappingComponent->GetOrCreateActionDelegate(FName("FireLeft")).AddUObject(this,&AHeroController::EndFire);
		}
		InputComponent->BindAxis("Turn",this,&AHeroController::Turn);
		InputComponent->BindAxis("LookUp",this,&AHeroController::LookUp);
		InputComponent->BindAxis("MoveForward",this,&AHeroController::Forward);
		InputComponent->BindAxis("MoveRight",this,&AHeroController::Right);
	}
}

void AHeroController::BeginPlay()
{
	Super::BeginPlay();
}


	
void AHeroController::Jump()
{
	if (ACharacter* MyCharacter = GetCharacter())
	{
		// MyCharacter->Jump();
		if (JumpTimes < MaxJumpTime)
		{
			FVector JumpImpulse = FVector(0,0,JumpVelocity);
			MyCharacter->LaunchCharacter(JumpImpulse,false,true);
			MyCharacter->bPressedJump = true;
			JumpTimes++;
		}
	}
}

void AHeroController::StopJump()
{
	if (ACharacter* MyCharacter = GetCharacter())
	{
		MyCharacter->bPressedJump = false;
	}
}

void AHeroController::Turn(float Value)
{
	if (Value != 0)
	{
		AddYawInput(Value);
	}
}

void AHeroController::LookUp(float Value)
{
	if (Value != 0)
	{
		AddPitchInput(Value);
	}
}

void AHeroController::Forward(float Value)
{
	if (ACharacter* MyCharacter = GetCharacter())
	{
		auto PMC = MyCharacter->GetCharacterMovement();
		if (!PMC->IsFalling())
		{
			const FRotator Rotation = GetControlRotation();
			const FVector Dir = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
			MyCharacter->AddMovementInput(Dir,Value * MoveSpeed,false);
		}
		else if (PMC->IsFalling() && Value * PMC->Velocity.X < 0.f) // TODO: Fix this aerial momentum shit, feel shitty as hell
		{
			PMC->AddForce(FVector(0.f,Value * 80000.f,0.0f));
		}
	}
}

void AHeroController::Right(float Value)
{
	if (ACharacter* MyCharacter = GetCharacter())
	{
		auto PMC = MyCharacter->GetCharacterMovement();
		if (!PMC->IsFalling())
		{
			const FRotator Rot = GetControlRotation();
			const FVector Dir = FRotationMatrix(Rot).GetUnitAxis(EAxis::Y);
			MyCharacter->AddMovementInput(Dir,Value * MoveSpeed,false);
		}
		else if (PMC->IsFalling() && Value * PMC->Velocity.Y < 0.f)
		{
			PMC->AddForce(FVector(Value * 80000.f,0.f,0.0f));
		}
	}
}


void AHeroController::ClearJumpTimes()
{
	JumpTimes = 0;
}

// TODO: DRY PRINCIPLE VIOLATION!!!
void AHeroController::TryActivatePrimary_Controller()
{
	if (AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(GetCharacter()))
	{
		HeroCharacter->GetSkillComponent()->TryActivateSkill(HeroCharacter,ESkillSlot::ESS_Primary);
	}
}

void AHeroController::TryActivateSecondary_Controller()
{
	if (AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(GetCharacter()))
	{
		HeroCharacter->GetSkillComponent()->TryActivateSkill(HeroCharacter,ESkillSlot::ESS_Secondary);
	}
}

void AHeroController::TryFire_R()
{
	if (AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(GetCharacter()))
	{
		if (auto WeaponComp = HeroCharacter->GetWeaponComponent())
		{
			WeaponComp->Fire_R(HeroCharacter);
			bIsFiring = true;
		}
	}
}

void AHeroController::TryFire_L()
{
	if (AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(GetCharacter()))
	{
		if (UWeaponComponent* WeaponComp = HeroCharacter->GetWeaponComponent())
		{
			WeaponComp->Fire_L(HeroCharacter);
			bIsFiring = true;
		}
		else UE_LOG(LogTemp, Warning, TEXT("WeaponComp FAILS"));
	}
}

void AHeroController::EndFire()
{
	bIsFiring = false;
}

void AHeroController::ReloadCall()
{
	if (auto Hero = Cast<AHeroCharacter>(GetCharacter()))
	{
		if (auto WeaponComp = Hero->GetWeaponComponent())
		{
			UE_LOG(LogTemp,Warning,TEXT("Start Reloading"));
			GetWorld()->GetTimerManager().SetTimer(
				WeaponComp->ReloadTimer,
				WeaponComp,
				&UWeaponComponent::Reload,
				WeaponComp->GetReloadTime(),
				false
			);
		}
	}
}
