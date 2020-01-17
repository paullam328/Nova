// Copywrite Shapeshifter 2019

#include "TransitionPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void ATransitionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	bEnableMouseOverEvents = false;
	bEnableMouseOverEvents = false;


	if (IsLocalController())
	{
		//UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		if (StageSwitchWipeWidgetClass != nullptr && StageSwitchWipeWidget == nullptr)
		{
			StageSwitchWipeWidget = CreateWidget<UStageSwitchWipeWidget>(this, StageSwitchWipeWidgetClass);
			StageSwitchWipeWidget->AddToViewport();
		}
	}
}