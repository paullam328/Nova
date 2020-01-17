// Copywrite Shapeshifter 2019

#include "CharacterSelectWidget.h"
#include "Button.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FightingGameMode.h"
#include "NetworkingGameInstance.h"
#include "FightingGameInstance.h"
#include "TextBlock.h"
#include "WidgetTree.h"
#include "Image.h"
#include "CharacterSelectPawn.h"
#include "CharacterSelectGameState.h"
#include "Kismet/GameplayStatics.h"
#include "EPlayerReady.h"
#include "CharacterSelectController.h"
#include "CharacterSelectGameState.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"
#include "AkAmbientSound.h"
#include "AkComponent.h"
#include "ProgressBar.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstance.h"
#include "WidgetAnimation.h"

#include "CharacterSelectionGameMode.h"
#include "CharacterSelectTimerState.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "CharacterPlayerState.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


UCharacterSelectWidget::UCharacterSelectWidget(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{ 
}

void UCharacterSelectWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	SetCharacterImgs();
	SetReadyTexts();
	TimerText->SetText(SetSecondsText());
	SetAlertText();
	SetTimeRadian(DeltaTime);

	HoverWwiseHandler();
	ResetLerpAlphaOnStage();
	Widget_OnLeaveReadySetFocus(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}

void UCharacterSelectWidget::HoldToLeave()
{
	if (leaveProgress < 1.0f)
	{
		if (LeaveProgressBar != nullptr)
		{
			leaveProgress += leaveValue;
			LeaveProgressBar->SetPercent(leaveProgress);
		}
	}
	else
	{
		// Leaving the Session
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			UNetworkingGameInstance* GI = Cast<UNetworkingGameInstance>(World->GetGameInstance());

			if (GI != nullptr)
			{
				GI->DisconnectFromServer();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Can't Logout")));
			}
		}		
	}
}

void UCharacterSelectWidget::ReleaseLeave()
{
	leaveProgress = 0.0f;
	LeaveProgressBar->SetPercent(leaveProgress);
}

void UCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	startButton = (UButton*)GetWidgetFromName(TEXT("StartButton"));
	startButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::StartButton);

	characterOne = (UButton*)GetWidgetFromName(TEXT("DemarkBTN"));
	characterOne->OnClicked.AddDynamic(this, &UCharacterSelectWidget::SelectCharacterOne);

	characterTwo = (UButton*)GetWidgetFromName(TEXT("MaskiumBTN"));
	characterTwo->OnClicked.AddDynamic(this, &UCharacterSelectWidget::SelectCharacterTwo);

	LeaveProgressBar = (UProgressBar*)GetWidgetFromName(TEXT("LeaveProgressBar"));

	startButton->SetIsEnabled(false);

	Player1SelectImg = (UImage*)(WidgetTree->FindWidget(FName("Player1Img")));

	Player2SelectImg = (UImage*)(WidgetTree->FindWidget(FName("Player2Img")));

	Player3SelectImg = (UImage*)(WidgetTree->FindWidget(FName("Player3Img")));

	Player4SelectImg = (UImage*)(WidgetTree->FindWidget(FName("Player4Img")));

	Player5SelectImg = (UImage*)(WidgetTree->FindWidget(FName("Player5Img")));

	Player6SelectImg = (UImage*)(WidgetTree->FindWidget(FName("Player6Img")));

	SelectImgs.Add(Player1SelectImg);
	SelectImgs.Add(Player2SelectImg);
	SelectImgs.Add(Player3SelectImg);
	SelectImgs.Add(Player4SelectImg);
	SelectImgs.Add(Player5SelectImg);
	SelectImgs.Add(Player6SelectImg);

	Player1Name = (UTextBlock*)(WidgetTree->FindWidget(FName("Player1Name")));
	Player2Name = (UTextBlock*)(WidgetTree->FindWidget(FName("Player2Name")));
	Player3Name = (UTextBlock*)(WidgetTree->FindWidget(FName("Player3Name")));
	Player4Name = (UTextBlock*)(WidgetTree->FindWidget(FName("Player4Name")));
	Player5Name = (UTextBlock*)(WidgetTree->FindWidget(FName("Player5Name")));
	Player6Name = (UTextBlock*)(WidgetTree->FindWidget(FName("Player6Name")));

	PlayerNames.Add(Player1Name);
	PlayerNames.Add(Player2Name);
	PlayerNames.Add(Player3Name);
	PlayerNames.Add(Player4Name);
	PlayerNames.Add(Player5Name);
	PlayerNames.Add(Player6Name);

	Player1ReadyText = (UTextBlock*)(WidgetTree->FindWidget(FName("Player1ReadyText")));
	Player2ReadyText = (UTextBlock*)(WidgetTree->FindWidget(FName("Player2ReadyText")));
	Player3ReadyText = (UTextBlock*)(WidgetTree->FindWidget(FName("Player3ReadyText")));
	Player4ReadyText = (UTextBlock*)(WidgetTree->FindWidget(FName("Player4ReadyText")));
	Player5ReadyText = (UTextBlock*)(WidgetTree->FindWidget(FName("Player5ReadyText")));
	Player6ReadyText = (UTextBlock*)(WidgetTree->FindWidget(FName("Player6ReadyText")));

	PlayerReadyTexts.Add(Player1ReadyText);
	PlayerReadyTexts.Add(Player2ReadyText);
	PlayerReadyTexts.Add(Player3ReadyText);
	PlayerReadyTexts.Add(Player4ReadyText);
	PlayerReadyTexts.Add(Player5ReadyText);
	PlayerReadyTexts.Add(Player6ReadyText);
	/*
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		Quote = World->SpawnActor<AAkAmbientSound>(DemarkQuoteClass, FVector(0,0,0), FRotator(0,0,0), SpawnParams);
	}
	*/

	AlertText = (UTextBlock*)(WidgetTree->FindWidget(FName("Alert")));
	AlertTextAnim = GetWidgetTreeOwningClass()->Animations[0];
	AlertTextAnim = GetWidgetTreeOwningClass()->Animations[1];

	TimerText = (UTextBlock*)(WidgetTree->FindWidget(FName("Time")));
	TimerImg = (UImage*)(WidgetTree->FindWidget(FName("Timer")));
	TimerMatDyn = TimerImg->GetDynamicMaterial();
	TimerImg->SetBrushFromMaterial(TimerMatDyn);
	TimerMatDyn->SetScalarParameterValue(FName(TEXT("Timer")), 1.0f);

	characterOne->SetKeyboardFocus();
	//GetOwningPlayer<ACharacterSelectController>()->Trigger_CharSelectMenu_AdjustSFXSlider(GetOwningPlayer());

	ReadyBanner = (UImage*)(WidgetTree->FindWidget(FName("ReadyBanner")));
	ReadyBannerButton = (UButton*)(WidgetTree->FindWidget(FName("ReadyButton")));
	ReadyBannerButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::SetReadyBannerColor_Ready);
	
	readyText = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_25"));

	HideBanner();
}


void UCharacterSelectWidget::StartButton()
{
	PostWWiseEvent(false, true);


	ACharacterPlayerState* CPS = GetOwningPlayer()->GetPlayerState<ACharacterPlayerState>();

	if (CPS != nullptr)
	{
		CPS->Trigger_SetPlayerReady(EPlayerReady::Ready);
		StartButton_SetColor(true);
		//Set widget focus
		Widget_SetUserFocus("One", UGameplayStatics::GetPlayerController(GetWorld(), 0));

		//CPS->Trigger_ServerTravel();
		CPS->Trigger_CheckReadyServerTravel();

	}
}

void UCharacterSelectWidget::StartButton_SetColor(bool IsReady)
{
	if (IsReady)
	{
		readyText->SetColorAndOpacity(FSlateColor(FLinearColor(0, 1, 0, 1)));
	}
	else
	{
		readyText->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 1)));
	}
}

void UCharacterSelectWidget::setReadyButton()
{
	UButton* readyButton = (UButton*)GetWidgetFromName(TEXT("StartButton"));
	if (readyButton->GetIsEnabled() == false)
	{
		readyButton->IsFocusable = true;
		GetWidgetFromName(TEXT("Characters"))->SetNavigationRule(EUINavigation::Down, EUINavigationRule::Explicit, "StartButton");
		readyButton->SetIsEnabled(true);

		if (readyButton->IsFocusable == true)
		{
			FString thingyToPrint = "True";
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
		}
		else
		{
			FString thingyToPrint = "False";
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
		}
	}
}

void UCharacterSelectWidget::SelectCharacter(int32 cNum)
{
	ACharacterPlayerState* CPS = GetOwningPlayer()->GetPlayerState<ACharacterPlayerState>();
	if (CPS != nullptr)
	{
		CPS->Trigger_SelectCharacter(cNum);
	}

	setReadyButton();

	FName charBordName = "";
	switch (cNum)
	{
	case 0:
		charBordName = "DemarkBTN";
		changeSelectBorder(2);
		break;
	case 1:
		charBordName = "MaskiumBTN";
		changeSelectBorder(1);
		break;
	default:
		break;
	}
	if (charBordName != "")
	{
		UButton* border = (UButton*)GetWidgetFromName(charBordName);
		border->SetStyle(selectedBorder);
		hasSelected = true;
	}

}

void UCharacterSelectWidget::SelectCharacterOne()
{
	PostWWiseEvent(false, true);

	SelectCharacter(0);


	UAkGameplayStatics::SetRTPCValue("CHAR_Num", 0, 0, UGameplayStatics::GetPlayerController(GetWorld(), 0));
	StartButton();
	UAkGameplayStatics::PostEvent(Quote, UGameplayStatics::GetPlayerController(GetWorld(), 0), false, "");
}

void UCharacterSelectWidget::SelectCharacterTwo()
{
	PostWWiseEvent(false, true);
	StartButton();
	SelectCharacter(1);

	UAkGameplayStatics::SetRTPCValue("CHAR_Num", 1, 0, UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UAkGameplayStatics::PostEvent(Quote, UGameplayStatics::GetPlayerController(GetWorld(), 0), false, "");
}

void UCharacterSelectWidget::SelectCharacterThree()
{
	PostWWiseEvent(false, true);

	SelectCharacter(2);
}

void UCharacterSelectWidget::SelectCharacterFour()
{
	PostWWiseEvent(false, true);

	SelectCharacter(3);
}

void UCharacterSelectWidget::SelectCharacterFive()
{
	PostWWiseEvent(false, true);

	SelectCharacter(4);
}

void UCharacterSelectWidget::SelectCharacterSix()
{
	PostWWiseEvent(false, true);

	SelectCharacter(5);
}

void UCharacterSelectWidget::changeSelectBorder(int32 cNum)
{
	if (cNum == 1)
	{
		FString name1 = ("DemarkBTN");
		UButton* border1 = (UButton*)GetWidgetFromName(FName(*name1));
		border1->SetStyle(staticBorder);
		
		ACharacterSelectController* PC = Cast<ACharacterSelectController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC != nullptr)
		{
			PC->SelectMaskium();
		}
	}
	else if (cNum == 2)
	{
		FString name2 = ("MaskiumBTN");
		UButton* border2 = (UButton*)GetWidgetFromName(FName(*name2));
		border2->SetStyle(staticBorder);

		ACharacterSelectController* PC = Cast<ACharacterSelectController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC != nullptr)
		{
			PC->SelectDemark();
		}
	}
}

void UCharacterSelectWidget::SetCharacterImgs()
{

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		ACharacterSelectGameState* GS = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());
		if (GS != nullptr)
		{ 
			for (int i = 0; i < SelectImgs.Num(); i++)
			{

				if (GS->PlayerArray.IsValidIndex(i) && SelectImgs.IsValidIndex(i))
				{
					ACharacterPlayerState* PlayerState = Cast< ACharacterPlayerState>(GS->PlayerArray[i]);

						switch (PlayerState->GetNumOfSelectedIndex())
						{
						case 0:
							SelectImgs[i]->SetBrushFromTexture(DemarkPic, false);
							break;
						case 1:
							SelectImgs[i]->SetBrushFromTexture(MaskiumPic, false);
							break;
						default:
							SelectImgs[i]->SetBrushFromTexture(NullPic, false);
							break;
						}
				}
				else
				{
					//edge case: player left char selection
					SelectImgs[i]->SetBrushFromTexture(NullPic, false);
				}
			}
		}
	}
}



void UCharacterSelectWidget::SetReadyTexts()
{
	ACharacterSelectGameState* GS = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		for (int j = 0; j < PlayerReadyTexts.Num(); j++)
		{
			if (PlayerReadyTexts.IsValidIndex(0) && SelectImgs.Num() == 6
				&& GS->PlayerArray.IsValidIndex(j))
			{
				ACharacterPlayerState* CPS = Cast<ACharacterPlayerState>(GS->PlayerArray[j]);
				switch (CPS->GetIsPlayerReady())
				{
				case EPlayerReady::Ready:
				{
					FString ready = "Ready";

					PlayerReadyTexts[j]->SetText(FText::FromString(*ready));
					PlayerReadyTexts[j]->SetColorAndOpacity(FLinearColor(0.0f, 1.0f, 0.0f));
				}
				break;
				case EPlayerReady::NotReady:
				{
					FString ready = "Not Ready";
					PlayerReadyTexts[j]->SetText(FText::FromString(*ready));
					PlayerReadyTexts[j]->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f));
				}
				break;
				case EPlayerReady::NonExistent:
				{
					FString sthWrong = "Sthwrong";
				}
				break;
				}

			}
			else
			{
				FString ready = "Not Exist";
				PlayerReadyTexts[j]->SetText(FText::FromString(*ready));
				PlayerReadyTexts[j]->SetColorAndOpacity(FLinearColor(0.3f, 0.3f, 0.3f));
			}

		}
	}
}


FText UCharacterSelectWidget::SetSecondsText()
{
	FString secStr = "";
	ACharacterSelectGameState* GM = Cast<ACharacterSelectGameState>(UGameplayStatics::GetGameState(GetWorld()));
	ACharacterSelectController* CSC = Cast<ACharacterSelectController>(GetOwningPlayer());
	if (GM != nullptr && CSC != nullptr)
	{
		//set timerSec:
		CSC->Server_GetTimer(); //set widget timer
		float sec = FMath::FloorToInt(Widget_timerSec) % maxSec;
		if (sec < 10 && sec >= 0)
		{
			secStr = "0";
			secStr.Append(FString::FromInt(sec));
		}
		else
		{
			secStr = FString::FromInt(sec);
		}

		//Visibility:
		if (GM->timerState == ECharacterSelectTimerState::WaitingForTwoPlayers
			|| GM->timerState == ECharacterSelectTimerState::StageWiping
			|| sec < 0)
		{
			TimerText->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			TimerText->SetVisibility(ESlateVisibility::Visible);
		}
	}
	return FText::FromString(secStr);
}

void UCharacterSelectWidget::SetTimeRadian(float DeltaTime)
{
	ACharacterSelectController* CSC = Cast<ACharacterSelectController>(GetOwningPlayer());
	if (CSC != nullptr)
	{
		float sec = Widget_timerSec;
		CSC->Server_GetShouldShowtimer(); //set widget timer
		if (!ShouldShowTimer)
		{
			TimerImg->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			TimerImg->SetVisibility(ESlateVisibility::Visible);

			FString name = "Percentage";
			float secPercentage = sec / maxSec;
			if (secPercentage < 10000 || secPercentage >= 0)
			{
				//lerp here:
				if (LerpTarget > secPercentage)
				{
					//reset lerp alpha
					LerpAlpha = 0;
					LerpStart = LerpTarget;
					LerpTarget = secPercentage;

				}
				else
				{
					//calc the lerp %
					LerpAlpha = LerpAlpha + DeltaTime * LerpSpeed;
				}


				if (TimerMatDyn != nullptr)
				{
					TimerMatDyn->SetScalarParameterValue(FName(TEXT("Timer")), UKismetMathLibrary::Lerp(LerpStart, LerpTarget, LerpAlpha));
				}
				else
				{
					TimerMatDyn = TimerImg->GetDynamicMaterial();
					TimerImg->SetBrushFromMaterial(TimerMatDyn);
					TimerMatDyn->SetScalarParameterValue(FName(TEXT("Timer")), secPercentage);
				}
			}
		}
	}
}

void UCharacterSelectWidget::SetAlertText()
{
	ACharacterSelectGameState* GM = Cast<ACharacterSelectGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GM != nullptr)
	{
		FString str = "";
		switch (GM->timerState)
		{
			case ECharacterSelectTimerState::WaitingForTwoPlayers:
				str = "Select Your Character";
				break;
			case ECharacterSelectTimerState::MainCountdown:
				str = "Waiting For More Players...";
				break;
			case ECharacterSelectTimerState::GetReady:
				str = "Get Ready";
				break;
		}
		AlertText->SetText(FText::FromString(*str));
		 
	}
}

void UCharacterSelectWidget::PlayAnim()
{
	PlayAnimation(AlertTextAnim, 0, 1);
}

void UCharacterSelectWidget::PlayReadyBannerAnim()
{
	PlayAnimation(AlertTextAnim, 0, 1);
}

void UCharacterSelectWidget::SetReadyBannerColor_Ready()
{
	UAkGameplayStatics::PostEvent(ReadyBannerVoice, GetOwningPlayer(), false, "");
	ReadyBanner->SetColorAndOpacity(FLinearColor(0, 1, 0, 1));
	ReadyBannerButton->SetColorAndOpacity(FLinearColor(0, 1, 0, 1));
	Cast<ACharacterSelectController>(GetOwningPlayer())->SetBannerReady();
}

void UCharacterSelectWidget::ReadyPopupBanner()
{
	ReadyBanner->SetVisibility(ESlateVisibility::Visible);
	ReadyBannerButton->SetVisibility(ESlateVisibility::Visible);

	ReadyBanner->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	ReadyBannerButton->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	PlayReadyBannerAnim();
	ReadyBannerButton->SetUserFocus(GetOwningPlayer());

	UAkGameplayStatics::PostEvent(UIReady, GetOwningPlayer(), false, "");

	//set isbannerready = true to notify characterselectctrl
	IsBannerReady = true;
}

void UCharacterSelectWidget::HideBanner()
{
	ReadyBanner->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
	ReadyBanner->SetVisibility(ESlateVisibility::Hidden);
	ReadyBannerButton->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
	ReadyBannerButton->SetVisibility(ESlateVisibility::Hidden);

	IsBannerReady = false;
	Widget_SetUserFocus("One", UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UCharacterSelectWidget::Widget_RemoveFromParent()
{
	RemoveFromParent();
}


void UCharacterSelectWidget::HoverWwiseHandler()
{
	if (startButton != nullptr &&
		characterOne != nullptr &&
		characterTwo != nullptr &&
		characterThree != nullptr &&
		characterFour != nullptr &&
		characterFive != nullptr &&
		characterSix != nullptr)
	{
		if (!startButtonHovered && startButton->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false);
			startButtonHovered = true;
			characterOneHovered = false;
			characterTwoHovered = false;
			characterThreeHovered = false;
			characterFourHovered = false;
			characterFiveHovered = false;
			characterSixHovered = false;
		}

		if (!characterOneHovered && characterOne->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false);
			startButtonHovered = false;
			characterOneHovered = true;
			characterTwoHovered = false;
			characterThreeHovered = false;
			characterFourHovered = false;
			characterFiveHovered = false;
			characterSixHovered = false;
		}

		if (!characterTwoHovered && characterTwo->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false);
			startButtonHovered = false;
			characterOneHovered = false;
			characterTwoHovered = true;
			characterThreeHovered = false;
			characterFourHovered = false;
			characterFiveHovered = false;
			characterSixHovered = false;
		}

		if (!characterThreeHovered && characterThree->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false);
			startButtonHovered = false;
			characterOneHovered = false;
			characterTwoHovered = false;
			characterThreeHovered = true;
			characterFourHovered = false;
			characterFiveHovered = false;
			characterSixHovered = false;
		}

		if (!characterFourHovered && characterFour->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false);
			startButtonHovered = false;
			characterOneHovered = false;
			characterTwoHovered = false;
			characterThreeHovered = false;
			characterFourHovered = true;
			characterFiveHovered = false;
			characterSixHovered = false;
		}

		if (!characterFiveHovered && characterFive->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false);
			startButtonHovered = false;
			characterOneHovered = false;
			characterTwoHovered = false;
			characterThreeHovered = false;
			characterFourHovered = false;
			characterFiveHovered = true;
			characterSixHovered = false;
		}

		if (!characterSixHovered && characterSix->HasUserFocus(GetOwningPlayer()))
		{
			PostWWiseEvent(true, false);
			startButtonHovered = false;
			characterOneHovered = false;
			characterTwoHovered = false;
			characterThreeHovered = false;
			characterFourHovered = false;
			characterFiveHovered = false;
			characterSixHovered = true;
		}
	}
}


void UCharacterSelectWidget::PostWWiseEvent(bool IsHover, bool IsSelect)
{
	if (IsHover)
	{
		UAkGameplayStatics::PostEvent(UIHover, GetOwningPlayer()->GetPlayerState<ACharacterPlayerState>(), false, "");

	}
	if (IsSelect)
	{
		UAkGameplayStatics::PostEvent(UISelect, GetOwningPlayer()->GetPlayerState<ACharacterPlayerState>(), false, "");

	}
}


void UCharacterSelectWidget::ResetLerpAlphaOnStage()
{
	ACharacterSelectGameState* GM = Cast<ACharacterSelectGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GM != nullptr)
	{
		FString str = "";
		switch (GM->timerState)
		{
			case ECharacterSelectTimerState::GetReady:
			{
				if (!IsGetReadyAlphaReset)
				{
					LerpTarget = 1;
					IsGetReadyAlphaReset = true;
				}
			}
			break;

			case ECharacterSelectTimerState::StageWiping:
			{
				if (!IsStageSwipeAlphaReset)
				{
					LerpTarget = 1;
					IsStageSwipeAlphaReset = true;
				}
			}
			break;
		}

	}
}

void UCharacterSelectWidget::Widget_SetUserFocus(FString WidgetToFocusStr, APlayerController* PC)
{
	
	if (WidgetToFocusStr == "One")
	{
		//characterOne->SetUserFocus(PC);
	}
	else if (WidgetToFocusStr == "Ready")
	{
		startButton->SetUserFocus(PC);
	}
}

void UCharacterSelectWidget::Widget_OnLeaveReadySetFocus(APlayerController* PC)
{
	if (HasUserFocus(PC) && !IsBannerReady)
	{
		Widget_SetUserFocus("One", PC);
	}
}


//Extra stuff:
void UCharacterSelectWidget::SetSelfNameGreen(int i)
{
	if (PlayerNames.IsValidIndex(i))
	{
		PlayerNames[i]->SetColorAndOpacity(FSlateColor(FLinearColor(0, 1, 0, 1)));
	}
	for (int j = 0; j < PlayerNames.Num(); j++)
	{
		if (j != i)
		{
			PlayerNames[j]->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 0.5f)));
		}
	}
}
