// Copywrite Shapeshifter 2019

#include "WinLoseScreen.h"
#include "WidgetTree.h"
#include "FighterBase.h"
#include "Condition.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "Image.h"
#include "WidgetAnimation.h"

//Make use of the parameter for local use:
void UWinLoseScreen::SetWinLoseText(ECondition cond)
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (cond == ECondition::Win)
		{
			WinImg->SetVisibility(ESlateVisibility::HitTestInvisible);
			LoseImg->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (cond == ECondition::Lose)
		{
			WinImg->SetVisibility(ESlateVisibility::Hidden);
			LoseImg->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		PlayAnimation(GetWidgetTreeOwningClass()->Animations[0], 0, 1);
	}
}

void UWinLoseScreen::NativeConstruct()
{
	WinLoseTextBox = (UEditableTextBox*)(WidgetTree->FindWidget(FName("WinLoseText")));
	WinImg = (UImage*)(WidgetTree->FindWidget(FName("WinImg")));
	LoseImg = (UImage*)(WidgetTree->FindWidget(FName("LoseImg")));
}