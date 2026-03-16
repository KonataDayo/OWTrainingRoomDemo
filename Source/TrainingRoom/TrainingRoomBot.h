#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrainingRoomBot.generated.h"

UCLASS()
class TRAININGROOM_API ATrainingRoomBot : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

public:
	ATrainingRoomBot();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
