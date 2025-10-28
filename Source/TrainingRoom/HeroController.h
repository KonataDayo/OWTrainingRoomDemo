#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroController.generated.h"

UCLASS()
class TRAININGROOM_API AHeroController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	class UInputMappingComponent* InputMappingComponent;

	UPROPERTY()
	class UInputMappingSubsystem* CachedMappingSubsystem;

	UFUNCTION(BlueprintCallable)
	void RebindPrimaryToQ_ForTestPurpose();

	bool bIsFiring = false;

protected:

	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Movement")
	float JumpVelocity;

	UPROPERTY(EditAnywhere, Category = "Movement")
	int32 MaxJumpTime = 2;

	int32 JumpTimes = 0;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 300.f;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	AHeroController();

	void Jump();
	void StopJump();
	void Turn(float Value);
	void LookUp(float Value);
	void Forward(float Value);
	void Right(float Value);
	void ClearJumpTimes();

	UFUNCTION(BlueprintCallable)
	void TryActivatePrimary_Controller();
	UFUNCTION(BlueprintCallable)
	void TryActivateSecondary_Controller();

	UFUNCTION(BlueprintCallable)
	void TryFire_R();
	UFUNCTION(BlueprintCallable)
	void TryFire_L();
	UFUNCTION()
	void EndFire();
	UFUNCTION()
	void ReloadCall();
};


