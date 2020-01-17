// Copywrite Shapeshifter 2019

#include "TimerScreen.h"
#include "WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "TextBlock.h"
#include "Image.h"
#include "WidgetAnimation.h"
#include "FighterController.h"

void UTimerScreen::NativeConstruct()
{
	MinutesTextBox = (UEditableTextBox*)(WidgetTree->FindWidget(FName("MinutesText")));
	SecondsTextBox = (UEditableTextBox*)(WidgetTree->FindWidget(FName("SecondsText")));

	MinutesTextBox = (UEditableTextBox*)(WidgetTree->FindWidget(FName("MinutesText")));
	SecondsTextBox = (UEditableTextBox*)(WidgetTree->FindWidget(FName("SecondsText")));

	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	for (int i = 0; i < WidgetClass->Animations.Num(); i++)
	{
		if (WidgetClass->Animations[i]->GetName() == "Swipe_Left_INST")
		{
			Swipe_Left = WidgetClass->Animations[i];
		}
		else if (WidgetClass->Animations[i]->GetName() == "Swipe_Right_INST")
		{
			Swipe_Right = WidgetClass->Animations[i];
		}
		else if (WidgetClass->Animations[i]->GetName() == "Swipe_Top_INST")
		{
			Swipe_Top = WidgetClass->Animations[i];
		}
		else if (WidgetClass->Animations[i]->GetName() == "Size_INST")
		{
			Size = WidgetClass->Animations[i];
		}
	}

	Three = (UImage*)(WidgetTree->FindWidget(FName("3")));
	Two = (UImage*)(WidgetTree->FindWidget(FName("2")));
	One = (UImage*)(WidgetTree->FindWidget(FName("1")));
	Go = (UImage*)(WidgetTree->FindWidget(FName("Go")));

}

void UTimerScreen::BP_Tick()
{
	SetInitialCDText();

}

FText UTimerScreen::SetMinutesText()
{
	Cast<AFighterController>(GetOwningPlayer())->Server_GetTimer();
	float min = Timer / 60;
	FString minStr = "0";
	minStr.Append(FString::FromInt(FMath::FloorToInt(min)));
	return FText::FromString(minStr);
}

FText UTimerScreen::SetSecondsText()
{
	Cast<AFighterController>(GetOwningPlayer())->Server_GetTimer();
	//float sec = FMath::FloorToInt(Timer) % 60;
	float sec = FMath::FloorToInt(Timer);
	FString secStr = "";
	/*if (sec < 10)
	{
		secStr = "0";
		secStr.Append(FString::FromInt(sec));
	}
	else
	{*/
		secStr = FString::FromInt(sec);
	//}
	return FText::FromString(secStr);
}

void UTimerScreen::SetInitialCDText()
{
	Cast<AFighterController>(GetOwningPlayer())->Server_GetTimer();
	Cast<AFighterController>(GetOwningPlayer())->Server_GetTimerState();

	if (TimerState == ETimerState::InitialCountdown)
	{
		int sec = FMath::FloorToInt(Timer) % 60;
		switch (sec)
		{
			case 3:
			{
				Three->SetVisibility(ESlateVisibility::Visible);
				Two->SetVisibility(ESlateVisibility::Hidden);
				One->SetVisibility(ESlateVisibility::Hidden);
				Go->SetVisibility(ESlateVisibility::Hidden);
				if (!isThreeAnimPlayed)
				{
					PlayAnimation(Swipe_Left, 0, 1);
					isThreeAnimPlayed = true;
				}
			}
			break;
			case 2:
			{
				Three->SetVisibility(ESlateVisibility::Hidden);
				Two->SetVisibility(ESlateVisibility::Visible);
				One->SetVisibility(ESlateVisibility::Hidden);
				Go->SetVisibility(ESlateVisibility::Hidden);
				if (!isTwoAnimPlayed)
				{
					PlayAnimation(Swipe_Right, 0, 1);
					isTwoAnimPlayed = true;
				}
			}
			break;
			case 1:
			{
				Three->SetVisibility(ESlateVisibility::Hidden);
				Two->SetVisibility(ESlateVisibility::Hidden);
				One->SetVisibility(ESlateVisibility::Visible);
				Go->SetVisibility(ESlateVisibility::Hidden);
				if (!isOneAnimPlayed)
				{
					PlayAnimation(Swipe_Top, 0, 1);
					isOneAnimPlayed = true;
				}
			}
			break;
			case 0:
			{
				Three->SetVisibility(ESlateVisibility::Hidden);
				Two->SetVisibility(ESlateVisibility::Hidden);
				One->SetVisibility(ESlateVisibility::Hidden);
				Go->SetVisibility(ESlateVisibility::Visible);
				if (!isGoAnimPlayed)
				{
					PlayAnimation(Size, 0, 1);
					isGoAnimPlayed = true;
				}
			}
			break;
			default:
				break;
		}
	}
}

ESlateVisibility UTimerScreen::SetThreeVisibility()
{

	ESlateVisibility visibility = ESlateVisibility::Visible;

	Two->SetVisibility(ESlateVisibility::Hidden);
	One->SetVisibility(ESlateVisibility::Hidden);
	Go->SetVisibility(ESlateVisibility::Hidden);

	if (!isThreeAnimPlayed)
	{
		PlayAnimation(Swipe_Left, 0, 1);
		isThreeAnimPlayed = true;
	}

	return visibility;
}

ESlateVisibility UTimerScreen::SetTwoVisibility()
{

	ESlateVisibility visibility = ESlateVisibility::Visible;

	Three->SetVisibility(ESlateVisibility::Hidden);
	One->SetVisibility(ESlateVisibility::Hidden);
	Go->SetVisibility(ESlateVisibility::Hidden);

	if (!isTwoAnimPlayed)
	{
		PlayAnimation(Swipe_Right, 0, 1);
		isTwoAnimPlayed = true;
	}
	return visibility;
}

ESlateVisibility UTimerScreen::SetOneVisibility()
{

	ESlateVisibility visibility = ESlateVisibility::Visible;
	
	Three->SetVisibility(ESlateVisibility::Hidden);
	Two->SetVisibility(ESlateVisibility::Hidden);
	Go->SetVisibility(ESlateVisibility::Hidden);

	if (!isOneAnimPlayed)
	{
		PlayAnimation(Swipe_Top, 0, 1);
		isOneAnimPlayed = true;
	}

	return visibility;
}

ESlateVisibility UTimerScreen::SetGoVisibility()
{

	ESlateVisibility visibility = ESlateVisibility::Visible;

	Three->SetVisibility(ESlateVisibility::Hidden);
	Two->SetVisibility(ESlateVisibility::Hidden);
	One->SetVisibility(ESlateVisibility::Hidden);

	if (!isGoAnimPlayed)
	{
		PlayAnimation(Size, 0, 1);
		isGoAnimPlayed = true;
	}

	return visibility;
}

ESlateVisibility UTimerScreen::SetMainCDVisibility()
{

	//Three->SetVisibility(ESlateVisibility::Visible);
	//Two->SetVisibility(ESlateVisibility::Visible);
	//One->SetVisibility(ESlateVisibility::Visible);
	//Go->SetVisibility(ESlateVisibility::Visible);

	ESlateVisibility visibility = ESlateVisibility::Collapsed;

	Cast<AFighterController>(GetOwningPlayer())->Server_GetTimerState();
	if (TimerState == ETimerState::MainCountdown)
	{
		isThreeAnimPlayed = false;
		isTwoAnimPlayed = false;
		isOneAnimPlayed = false;
		isGoAnimPlayed = false;

		Three->SetVisibility(ESlateVisibility::Hidden);
		Two->SetVisibility(ESlateVisibility::Hidden);
		One->SetVisibility(ESlateVisibility::Hidden);
		Go->SetVisibility(ESlateVisibility::Hidden);

		visibility = ESlateVisibility::HitTestInvisible;
	}
	else
	{
		visibility = ESlateVisibility::Hidden; //To prevent the bug that the user can modify the text
	}
	return visibility;
}

ESlateVisibility UTimerScreen::SetInitialCDVisibility()
{
	ESlateVisibility visibility = ESlateVisibility::Collapsed;
	Cast<AFighterController>(GetOwningPlayer())->Server_GetTimerState();
	if (TimerState == ETimerState::InitialCountdown)
	{
		SetInitialCDText();
		visibility = ESlateVisibility::HitTestInvisible; //To prevent the bug that the user can modify the text
	}
	else
	{
		visibility = ESlateVisibility::Hidden;
	}
	return visibility;
}

ESlateVisibility UTimerScreen::SetFinalVisibility()
{
	ESlateVisibility visibility = ESlateVisibility::Collapsed;
	Cast<AFighterController>(GetOwningPlayer())->Server_GetTimerState();
	if (TimerState == ETimerState::FinalStage)
	{
		visibility = ESlateVisibility::HitTestInvisible; //To prevent the bug that the user can modify the text
	}
	else
	{
		visibility = ESlateVisibility::Hidden;
	}
	return visibility;
}
