// Copywrite Shapeshifter 2019

#include "gamePlayUserHUD.h"
#include "gamePlayUserWidget.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FighterController.h"

AgamePlayUserHUD::AgamePlayUserHUD()
{

}

void AgamePlayUserHUD::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass)
	{
		widget = CreateWidget<UgamePlayUserWidget>(GetWorld(), WidgetClass);

		if (widget)
		{
			AFighterController* con = Cast<AFighterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			widget->AddToViewport();
			unPause();
			if (con != nullptr)
			{
				con->RunGamePlayWidget();
			}
		}
	}
}

void AgamePlayUserHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AgamePlayUserHUD::Pause()
{
	FString thingyToPrint = "called Pause";

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
	if (widget != nullptr)
	{
		UWidget* pauseScreen = widget->GetWidgetFromName("PauseScreen");

		if (isPaused == false)
		{
			// Pause
			FString thingyToPrint = "Pause";
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
			if (pauseScreen != nullptr)
			{
				pauseScreen->SetVisibility(ESlateVisibility::Visible);
				GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(widget->GetSlateWidgetFromName("ResumeBTN")));
				isPaused = true;
			}
		}
		else
		{
			// Unpause
			FString thingyToPrint = "unPause";
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
			pauseScreen->SetVisibility(ESlateVisibility::Hidden);
			widget->GetWidgetFromName("OptionsMenu")->SetVisibility(ESlateVisibility::Hidden);
			GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
			isPaused = false;
		}
	}
}

void AgamePlayUserHUD::unPause()
{
	widget->GetWidgetFromName("PauseScreen")->SetVisibility(ESlateVisibility::Hidden);
	GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
}
