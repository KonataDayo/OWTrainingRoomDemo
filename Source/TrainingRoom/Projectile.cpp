#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	SetLifeSpan(7.f);
	if (!ProjectileHitBox)
	{
		ProjectileHitBox = NewObject<UBoxComponent>(GetInstigator());
	}
	if (bIsSkill) ProjectileHitBox->OnComponentBeginOverlap.AddDynamic(this,&AProjectile::OnOverlapBegin);
}

void AProjectile::SetIsSkill(bool IsSkill)
{
	bIsSkill = IsSkill;
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

