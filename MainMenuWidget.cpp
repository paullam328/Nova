// Copywrite Shapeshifter 2019

#include "MainMenuWidget.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Image.h"
#include "Button.h"
#include "NetworkingGameInstance.h"
#include "Slider.h"
#include "GameFramework/PlayerController.h"
//#include "VerticalBox.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "MainMenuController.h"
#include "LevelMusicManager.h"
#include "Kismet/GameplayStatics.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();


	Logo = (UImage*)GetWidgetFromName(TEXT("BackGround"));
	DynMat = Logo->GetDynamicMaterial();
	Logo->SetBrushFromMaterial(DynMat);
	DynMat->SetScalarParameterValue(Frame, 0.0);

	/*TSharedPtr<SWidget> widgetToFocus = GetSlateWidgetFromName(TEXT("PlayBTN"));
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(widgetToFocus));*/

	playBTN = (UButton*)GetWidgetFromName(TEXT("PlayBTN"));
	playBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayBTNClick);

	optionsBTN = (UButton*)GetWidgetFromName(TEXT("OptionsBTN"));
	optionsBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::OptionsBTNClick);

	quitBTN = (UButton*)GetWidgetFromName(TEXT("QuitBTN"));
	quitBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitBTNClick);

	createGameBTN = (UButton*)GetWidgetFromName(TEXT("CreateGameBTN"));
	createGameBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateGameBTNClick);

	findGameBTN = (UButton*)GetWidgetFromName(TEXT("FindGameBTN"));
	findGameBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::FindGameBTNClick);

	backBTN = (UButton*)GetWidgetFromName(TEXT("BackBTN"));
	backBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::BackBTNClick);

	optionsBackBTN = (UButton*)GetWidgetFromName(TEXT("OptionsBackBTN"));
	optionsBackBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::OptionsBackBTNClick);

	findGameBackBTN = (UButton*)GetWidgetFromName(TEXT("BackButton"));
	findGameBackBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::FindGameBackBTNClick);

	CreateGameBackBTN = (UButton*)GetWidgetFromName(TEXT("CreateGameBackBTN"));
	CreateGameBackBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateGameBackBTNClick);

	SinglePlayerBTN = (UButton*)GetWidgetFromName(TEXT("SinglePlayerBTN"));
	SinglePlayerBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::SinglePlayerBTNClick);

	RefreshBTN = (UButton*)GetWidgetFromName(TEXT("RefreshButton"));
	RefreshBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::RefreshBTNClick);

	gammaSlider = (USlider*)GetWidgetFromName(TEXT("GammaSlider"));
	gammaSlider->OnValueChanged.AddDynamic(this, &UMainMenuWidget::gammaSliderValueChanged);

	musicSlider = (USlider*)GetWidgetFromName(TEXT("MusicSlider"));
	musicSlider->OnValueChanged.AddDynamic(this, &UMainMenuWidget::musicSliderValueChanged);

	sfxSlider = (USlider*)GetWidgetFromName(TEXT("SFXSlider"));
	sfxSlider->OnValueChanged.AddDynamic(this, &UMainMenuWidget::sfxSliderValueChanged);

	voiceSlider = (USlider*)GetWidgetFromName(TEXT("VoiceSlider"));
	voiceSlider->OnValueChanged.AddDynamic(this, &UMainMenuWidget::voiceSliderValueChanged);

	ControllsBTN = (UButton*)GetWidgetFromName(TEXT("ControllsBTN"));
	ControllsBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::ControllsBTNClick);

	ControllsBackBTN = (UButton*)GetWidgetFromName(TEXT("ControllsBackBTN"));
	ControllsBackBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::ControllsBackBTNClick);

	CreditsBTN = (UButton*)GetWidgetFromName(TEXT("CreditsBTN"));
	CreditsBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::CreditsBTNClick);

	CreditsBackBTN = (UButton*)GetWidgetFromName(TEXT("CreditsBackBTN"));
	CreditsBackBTN->OnClicked.AddDynamic(this, &UMainMenuWidget::CreditsBackBTNClick);

}


void UMainMenuWidget::NativeTick(const FGeometry & MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (DynMat->K2_GetScalarParameterValue(Frame) < 0.91)
	{
		float val = DynMat->K2_GetScalarParameterValue(Frame) + fps;
		DynMat->SetScalarParameterValue(Frame, val);
	}

	HoverWwiseHandler();

}

void UMainMenuWidget::PlayBTNClick()
{
	PostWWiseEvent(false, true, false);
	ButtonClick("FirstScreen", "SecondScreen", "CreateGameBTN");
}

void UMainMenuWidget::OptionsBTNClick()
{
	PostWWiseEvent(false, true, false);
	ButtonClick("FirstScreen", "OptionsScreen", "GammaSlider");
}

void UMainMenuWidget::QuitBTNClick()
{
	PostWWiseEvent(false, true, false);
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}

void UMainMenuWidget::CreateGameBTNClick()
{
	PostWWiseEvent(false, true, false);
	ButtonClick("SecondScreen", "CreateGameScreen", "CreateGameBTN2");
}

void UMainMenuWidget::FindGameBTNClick()
{
	PostWWiseEvent(false, true, false);
	ButtonClick("SecondScreen", "ServerBrowserScreen", "RefreshButton");
}

void UMainMenuWidget::BackBTNClick()
{
	PostWWiseEvent(false, false, true);
	ButtonClick("SecondScreen", "FirstScreen", "PlayBTN");
}

void UMainMenuWidget::OptionsBackBTNClick()
{
	PostWWiseEvent(false, false, true);
	ButtonClick("OptionsScreen", "FirstScreen", "PlayBTN");
}

void UMainMenuWidget::gammaSliderValueChanged(float value)
{
	FString Value1 = "gamma ";
	FString Value2 = FString::SanitizeFloat(gammaSlider->GetValue() * 10);
	FString finalValue = Value1 + Value2;

	GetOwningPlayer()->ConsoleCommand(finalValue);
}

void UMainMenuWidget::musicSliderValueChanged(float value)
{
	AMainMenuController* MMC = Cast<AMainMenuController>(GetOwningPlayer());
	if (MMC != nullptr)
	{
		Cast<AMainMenuController>(GetOwningPlayer())->SetMusSliderVal(value);

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelMusicManager::StaticClass(), FoundActors);

		if (FoundActors.IsValidIndex(0))
		{
			Cast<ALevelMusicManager>(FoundActors[0])->MainMenu_AdjustMusSlider(value);
		}
	}
}

void UMainMenuWidget::sfxSliderValueChanged(float value)
{
	Cast<AMainMenuController>(GetOwningPlayer())->SetSFXSliderVal(value);
	MainMenu_AdjustSFXSlider(value);
}

void UMainMenuWidget::voiceSliderValueChanged(float value)
{
	Cast<AMainMenuController>(GetOwningPlayer())->SetVoiceSliderVal(value);
}

void UMainMenuWidget::FindGameBackBTNClick()
{
	PostWWiseEvent(false, true, false);
	ButtonClick("ServerBrowserScreen", "SecondScreen", "FindGameBTN");
}

void UMainMenuWidget::CreateGameBackBTNClick()
{
	PostWWiseEvent(false, true, false);
	ButtonClick("CreateGameScreen", "SecondScreen", "CreateGameBTN");
}

void UMainMenuWidget::ButtonClick(FName thingToHide, FName thingToShow, FName thingToFocus)
{
	GetWidgetFromName(thingToHide)->SetVisibility(ESlateVisibility::Hidden);
	GetWidgetFromName(thingToHide)->SetIsEnabled(false);
	GetWidgetFromName(thingToShow)->SetVisibility(ESlateVisibility::Visible);
	GetWidgetFromName(thingToShow)->SetIsEnabled(true);
	TSharedPtr<SWidget> widgetToFocus = GetSlateWidgetFromName(thingToFocus);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(widgetToFocus));
}

void UMainMenuWidget::SinglePlayerBTNClick()
{
	PostWWiseEvent(false, true, false);
}

void UMainMenuWidget::RefreshBTNClick()
{
	PostWWiseEvent(false, true, false);
}

void UMainMenuWidget::ControllsBTNClick()
{
	PostWWiseEvent(false, true, false);
	ButtonClick("FirstScreen", "ControlsScreen", "ControllsBackBTN");
}

void UMainMenuWidget::ControllsBackBTNClick()
{
	PostWWiseEvent(false, true, false);
	ButtonClick("ControlsScreen", "FirstScreen", "ControllsBTN");
}

void UMainMenuWidget::CreditsBTNClick()
{
	ButtonClick("FirstScreen", "CreditsScreen", "CreditsBackBTN");
}

void UMainMenuWidget::CreditsBackBTNClick()
{
	ButtonClick("CreditsScreen", "FirstScreen", "CreditsBTN");
}


void UMainMenuWidget::PostWWiseEvent(bool IsHover, bool IsSelect, bool IsBack)
{
	if (IsHover)
	{
		UAkGameplayStatics::PostEvent(UIHover, GetOwningPlayer(), false, "");

	}
	if (IsSelect)
	{
		UAkGameplayStatics::PostEvent(UISelect, GetOwningPlayer(), false, "");

	}
	if (IsBack)
	{
		UAkGameplayStatics::PostEvent(UIBack, GetOwningPlayer(), false, "");

	}
}

void UMainMenuWidget::HoverWwiseHandler()
{
	if (playBTN != nullptr &&
		optionsBTN != nullptr &&
		quitBTN != nullptr &&
		createGameBTN != nullptr &&
		findGameBTN != nullptr &&
		backBTN != nullptr &&
		optionsBackBTN != nullptr &&
		findGameBackBTN != nullptr &&
		CreateGameBackBTN != nullptr &&
		SinglePlayerBTN != nullptr &&
		RefreshBTN != nullptr &&
		ControllsBTN != nullptr &&
		ControllsBackBTN != nullptr)
	{
		if (!playHovered && playBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = true;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!optionsHovered && optionsBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = true;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!quitHovered && quitBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = true;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!createGameHovered && createGameBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = true;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!findGameHovered && findGameBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = true;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!backHovered && backBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = true;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!optionsBackHovered && optionsBackBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = true;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!findGameBackHovered && findGameBackBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = true;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!CreateGameBackHovered && CreateGameBackBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = true;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!SinglePlayerHovered && SinglePlayerBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = true;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!RefreshHovered && RefreshBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = true;
			ControllsHovered = false;
			ControllsBackHovered = false;
		}

		if (!ControllsHovered && ControllsBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = true;
			ControllsBackHovered = false;
		}

		if (!ControllsBackHovered && ControllsBackBTN->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false, false);
			playHovered = false;
			optionsHovered = false;
			quitHovered = false;
			createGameHovered = false;
			findGameHovered = false;
			backHovered = false;
			optionsBackHovered = false;
			findGameBackHovered = false;
			CreateGameBackHovered = false;
			SinglePlayerHovered = false;
			RefreshHovered = false;
			ControllsHovered = false;
			ControllsBackHovered = true;
		}
	}
}

void UMainMenuWidget::MainMenu_AdjustSFXSlider(float SFXSliderVal)
{
	UAkGameplayStatics::SetOutputBusVolume(SFXSliderVal, GetOwningPlayer());
}