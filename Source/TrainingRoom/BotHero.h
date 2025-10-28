#pragma once

#include "CoreMinimal.h"
#include "HeroCharacter.h"
#include "TimerManager.h"
#include "BotHero.generated.h"

UCLASS()
class TRAININGROOM_API ABotHero : public AHeroCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float RegularFireInterval = 2.f;

protected:
	virtual void BeginPlay() override;
public:

	void FireRegularly();

	FTimerDelegate TimerDel;

	FTimerHandle RegularFireTimer;

};
