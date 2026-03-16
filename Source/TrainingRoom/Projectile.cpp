#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DamageSystem.h"
#include "ObjectPoolSubsystem.h"
#include "TimerManager.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	RootComponent = ProjectileMesh;
	ProjectileHitBox->SetupAttachment(RootComponent);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileHitBox->SetGenerateOverlapEvents(true);
	ProjectileHitBox->SetCollisionObjectType(ECC_EngineTraceChannel1);
	ProjectileHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileHitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (!ProjectileHitBox)
	{
		ProjectileHitBox = NewObject<UBoxComponent>(GetInstigator());
	}

	if (bIsSkill) ProjectileHitBox->OnComponentBeginOverlap.AddDynamic(this,&AProjectile::OnOverlapBegin);
	//else ProjectileHitBox->OnComponentBeginOverlap.AddDynamic(this,&AProjectile::HandleProjectileOverlap);
}

void AProjectile::SetIsSkill(bool IsSkill)
{
	bIsSkill = IsSkill;
}

void AProjectile::SetOwningWeapon(UWeapon* Weapon)
{
	if (!Weapon)
	{
		UE_LOG(LogTemp,Error,TEXT("[%s]Failed to set owning weapon"),*GetName());
		return;
	}
	OwningWeapon = Weapon;
}

void AProjectile::OnActivate_Implementation()
{
	IPoolable::OnActivate_Implementation();
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	if (ProjectileMovement) ProjectileMovement->Activate();
	GetWorld()->GetTimerManager().SetTimer(ExpireTimer,this,&AProjectile::ExpireHandler,ExpireTime);
	// delegates
	if (ProjectileHitBox)
	{
		
	}
}

void AProjectile::ExpireHandler()
{
#if WITH_EDITOR
	//UE_LOG(LogTemp,Display,TEXT("[%s] Expire, Now returning to pool"),*GetName());
#endif
	IPoolable::Execute_OnDeactivate(this);
	GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->Return(this);
}

void AProjectile::OnDeactivate_Implementation()
{
	IPoolable::OnDeactivate_Implementation();
	//UE_LOG(LogTemp, Display, TEXT("[%s](Poolable Interface) On Deactivate"), *GetName());
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Deactivate();
	}
	// delegates
	if (ProjectileHitBox)
	{
		ProjectileHitBox->OnComponentBeginOverlap.Clear();
	}
}

void AProjectile::InitializeVelocity(FVector FireDirection)
{
	if (!ProjectileMovement) return;
	ProjectileMovement->Velocity = FireDirection.GetSafeNormal() * ProjectileMovement->InitialSpeed;
}

// Skill Handler
void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnSkillProjectileActivate.Broadcast(this,OverlappedComp,OtherActor,OtherComp,OtherBodyIndex,bFromSweep,SweepResult,SkillSpec);
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::InitSkill(const FSkillSpec& InSpec)
{
	bInUse = true;
	SkillSpec = InSpec;
}


UBoxComponent* AProjectile::GetHitBox() const
{
	return ProjectileHitBox;
}

EProjectileDamageType AProjectile::GetDamageType() const
{
	return DamageType;
}

// bullet/Fire Handler(NOT BEING USED!)
void AProjectile::HandleProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
#if WITH_EDITOR
	UE_LOG(LogTemp,Error,TEXT("WHY THE FUCK WOULD THIS BE CALLED"));
#endif
}
