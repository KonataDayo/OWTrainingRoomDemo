#include "HealthBarWidgetComponent.h"
#include "HealthBarWidget.h"

void UHealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetUserWidgetObject())
	{
		InitWidget();
	}
}

UUserWidget* UHealthBarWidgetComponent::GetOrCreateWidget(APawn* Pawn)
{
	// need to fix this
	if (!HealthBarWidgetClass) return nullptr;
	if (!HealthBarWidget)
	{
		UUserWidget* UW = CreateWidget<UUserWidget>(Pawn->GetWorld(),HealthBarWidgetClass);
		if (UHealthBarWidget* HBWidget = Cast<UHealthBarWidget>(UW))
		{
			if (auto PC = Cast<APlayerController>(Pawn->GetController())) HBWidget->SetOwningPlayer(PC);
			HealthBarWidget = HBWidget;
		}
	}
	return HealthBarWidget;
}
