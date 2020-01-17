// Copywrite Shapeshifter 2019

#include "gamePlayUserWidget.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Button.h"
#include "gamePlayUserHUD.h"
#include "Slider.h"
//#include "../Plugins/AdvancedSessions/Source/AdvancedSessions/Classes/AdvancedSessionsLibrary.h"
//#include "../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"



UgamePlayUserWidget::UgamePlayUserWidget(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UgamePlayUserWidget::Tick(float InDeltaTime, FGeometry MyGeometry)
{
	Super::Tick(MyGeometry, InDeltaTime);
	/*FString thingyToPrint = "widgetTick";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));*/
}

void UgamePlayUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FString thingyToPrint = "widgetBegin";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));

	
	UButton* resumeBTN = (UButton*)GetWidgetFromName(TEXT("ResumeBTN"));
	resumeBTN->OnClicked.AddDynamic(this, &UgamePlayUserWidget::resumeButton);
	UButton* quitBTN = (UButton*)GetWidgetFromName(TEXT("QuitBTN"));
	quitBTN->OnClicked.AddDynamic(this, &UgamePlayUserWidget::quitButton);
	UButton* optionsBTN = (UButton*)GetWidgetFromName(TEXT("OptionsBTN"));
	optionsBTN->OnClicked.AddDynamic(this, &UgamePlayUserWidget::optionsButton);
	UButton* optionsBackBTN = (UButton*)GetWidgetFromName(TEXT("ReturnButton"));
	optionsBackBTN->OnClicked.AddDynamic(this, &UgamePlayUserWidget::optionsBackButton);
	gammaSlider = (USlider*)GetWidgetFromName(TEXT("GammaSlider"));
	gammaSlider->OnValueChanged.AddDynamic(this, &UgamePlayUserWidget::gammaSliderValueChanged);

}

void UgamePlayUserWidget::ButtonClick(FName thingToHide, FName thingToShow, FName thingToFocus)
{
	GetWidgetFromName(thingToHide)->SetVisibility(ESlateVisibility::Hidden);
	GetWidgetFromName(thingToHide)->SetIsEnabled(false);
	GetWidgetFromName(thingToShow)->SetVisibility(ESlateVisibility::Visible);
	GetWidgetFromName(thingToShow)->SetIsEnabled(true);
	TSharedPtr<SWidget> widgetToFocus = GetSlateWidgetFromName(thingToFocus);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(widgetToFocus));
}

void UgamePlayUserWidget::resumeButton()
{
	AgamePlayUserHUD* HUD = Cast<AgamePlayUserHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (HUD != nullptr)
	{
		UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
		UGameplayStatics::GetPlayerController(this, 0)->bEnableClickEvents = false;
		UGameplayStatics::GetPlayerController(this, 0)->bEnableMouseOverEvents = false;
		HUD->Pause();
	}
}

void UgamePlayUserWidget::quitButton()
{
	//UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this,0),EQuitPreference::Quit, false);
}

void UgamePlayUserWidget::optionsButton()
{
	ButtonClick("PauseScreen", "OptionsMenu", "GammaSlider");
}

void UgamePlayUserWidget::optionsBackButton()
{
	ButtonClick("OptionsMenu", "PauseScreen", "ResumeBTN");
}

void UgamePlayUserWidget::gammaSliderValueChanged(float value)
{
	FString Value1 = "gamma ";
	FString Value2 = FString::SanitizeFloat(gammaSlider->GetValue() * 10);
	FString finalValue = Value1 + Value2;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *finalValue));

	GetOwningPlayer()->ConsoleCommand(finalValue);
}
