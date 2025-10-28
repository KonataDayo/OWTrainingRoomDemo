#pragma once

#include "CoreMinimal.h"
#include "HeroCharacter.h"
#include "Tracer.generated.h"

UCLASS()
class TRAININGROOM_API ATracer : public AHeroCharacter
{
	GENERATED_BODY()

	float RecallTimer = 0.f;

	void UpdateRecallInfo(float DeltaTime);

	UPROPERTY(EditAnywhere,Category = "Recall")
	float SampleInterval = 0.1f;

	float TimeSinceLastSampled = 0.f;

	float MAX_INFO_SIZE;

protected:

	virtual void Tick(float DeltaSeconds) override;

public:

	void ResetRecallTimer();

	ATracer();

};
