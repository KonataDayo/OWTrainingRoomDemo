#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class TRAININGROOM_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	UPROPERTY()
	class UHealthBarWidget* HealthBarWidget;

	UPROPERTY(EditAnywhere,Category = "HealthBar")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;

protected:
	virtual void BeginPlay() override;

public:

	UUserWidget* GetOrCreateWidget(APawn* Pawn);
};

