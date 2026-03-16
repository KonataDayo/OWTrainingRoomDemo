#include "TrainingRoomBot.h"
#include "Components/SkeletalMeshComponent.h"

ATrainingRoomBot::ATrainingRoomBot()
{
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = SkeletalMesh;
}

void ATrainingRoomBot::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrainingRoomBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrainingRoomBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

