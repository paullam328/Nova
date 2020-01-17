// Copywrite Shapeshifter 2019

#include "MiniLeaderboardWidget.h"
#include "Engine.h"
#include "FightingGameState.h"
#include "FighterBase.h"
#include "TextBlock.h"
#include "FighterController.h"

void UMiniLeaderboardWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	UpdateLeaderboardRankings();
}

void UMiniLeaderboardWidget::UpdateLeaderboardRankings()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		//AFightingGameState* FGS = Cast<AFightingGameState>(World->GetGameState());
		//if (FGS != nullptr)
		//{
		//	MiniLeaderboardInfoPriorityHeap.Empty();
		//	for (int i = 0; i < FGS->survivingPlayers.Num(); i++)
		//	{
		//		
		//		AFighterBase* FB = Cast<AFighterBase>(FGS->survivingPlayers[i]);

		AFighterController* FC = Cast<AFighterController>(GetOwningPlayer());
		if (FC != nullptr)
		{
			FC->Multicast_GetSurvivingPlayers();

			for (int i = 0; i < FC->FighterBases.Num(); i++)
			{
				AFighterBase* FB = Cast<AFighterBase>(FC->FighterBases[i]);
				if (FB != nullptr)
				{
					APlayerState* PS = FB->GetPlayerState();
					if (PS != nullptr) //It may potentially crash on client otherwise
					{
						FMiniLeaderboardInfo info;
						info.SetName(PS->GetPlayerName());
						info.SetHealth(FB->GetHealth());
						MiniLeaderboardInfoPriorityHeap.HeapPush(info, FMiniLeaderboardInfoPredicate());
					}
				}
			}
		}

		SortUpdateMiniLeaderboard();
		//}
	}
}

void UMiniLeaderboardWidget::SortUpdateMiniLeaderboard()
{
	//Hide the unnecessary ones


	for (int i = Cast<AFighterController>(GetOwningPlayer())->FighterBases.Num(); i < RankTexts.Num(); i++)
	{
		RankTexts[i]->SetVisibility(ESlateVisibility::Hidden);
		UsernameTexts[i]->SetVisibility(ESlateVisibility::Hidden);
		RankTexts1[i]->SetVisibility(ESlateVisibility::Hidden);
		UsernameTexts1[i]->SetVisibility(ESlateVisibility::Hidden);
		RankTexts2[i]->SetVisibility(ESlateVisibility::Hidden);
		UsernameTexts2[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	//Get the ranking:
	AFighterController* FC = Cast<AFighterController>(GetOwningPlayer());
	if (FC != nullptr)
	{
		for (int i = 0; i < MiniLeaderboardInfoPriorityHeap.Num(); i++)
		{
			if (PlayerName == MiniLeaderboardInfoPriorityHeap[i].GetName())
			{
				FC->MiniLeaderboard_FetchRanking(i);
			}
		}
	}

	//Pull from heap:

	for (int i = 0; i < MiniLeaderboardInfoPriorityHeap.Num(); i++)
	{
		FMiniLeaderboardInfo Next;
		MiniLeaderboardInfoPriorityHeap.HeapPop(Next, FMiniLeaderboardInfoPredicate());
		RankTexts[i]->SetVisibility(ESlateVisibility::Visible);

		UsernameTexts[i]->SetVisibility(ESlateVisibility::Visible);
		UsernameTexts[i]->SetText(FText::FromString(*Next.GetName()));
		UsernameTexts1[i]->SetVisibility(ESlateVisibility::Visible);
		UsernameTexts1[i]->SetText(FText::FromString(*Next.GetName()));
		UsernameTexts2[i]->SetVisibility(ESlateVisibility::Visible);
		UsernameTexts2[i]->SetText(FText::FromString(*Next.GetName()));
	}
}

void UMiniLeaderboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Rank1Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank1"));
	Rank1Text->SetVisibility(ESlateVisibility::Hidden);

	Rank2Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank2"));
	Rank2Text->SetVisibility(ESlateVisibility::Hidden);

	Rank3Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank3"));
	Rank3Text->SetVisibility(ESlateVisibility::Hidden);

	Rank4Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank4"));
	Rank4Text->SetVisibility(ESlateVisibility::Hidden);

	Rank5Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank5"));
	Rank5Text->SetVisibility(ESlateVisibility::Hidden);

	Rank6Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank6"));
	Rank6Text->SetVisibility(ESlateVisibility::Hidden);

	RankTexts.Add(Rank1Text);
	RankTexts.Add(Rank2Text);
	RankTexts.Add(Rank3Text);
	RankTexts.Add(Rank4Text);
	RankTexts.Add(Rank5Text);
	RankTexts.Add(Rank6Text);

	Rank11Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank11"));
	Rank11Text->SetVisibility(ESlateVisibility::Hidden);

	Rank21Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank21"));
	Rank21Text->SetVisibility(ESlateVisibility::Hidden);

	Rank31Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank31"));
	Rank31Text->SetVisibility(ESlateVisibility::Hidden);

	Rank41Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank41"));
	Rank41Text->SetVisibility(ESlateVisibility::Hidden);

	Rank51Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank51"));
	Rank51Text->SetVisibility(ESlateVisibility::Hidden);

	Rank61Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank61"));
	Rank61Text->SetVisibility(ESlateVisibility::Hidden);

	RankTexts1.Add(Rank11Text);
	RankTexts1.Add(Rank21Text);
	RankTexts1.Add(Rank31Text);
	RankTexts1.Add(Rank41Text);
	RankTexts1.Add(Rank51Text);
	RankTexts1.Add(Rank61Text);

	Rank12Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank12"));
	Rank12Text->SetVisibility(ESlateVisibility::Hidden);

	Rank22Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank22"));
	Rank22Text->SetVisibility(ESlateVisibility::Hidden);

	Rank32Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank32"));
	Rank32Text->SetVisibility(ESlateVisibility::Hidden);

	Rank42Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank42"));
	Rank42Text->SetVisibility(ESlateVisibility::Hidden);

	Rank52Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank52"));
	Rank52Text->SetVisibility(ESlateVisibility::Hidden);

	Rank62Text = (UTextBlock*)GetWidgetFromName(TEXT("Rank62"));
	Rank62Text->SetVisibility(ESlateVisibility::Hidden);

	RankTexts2.Add(Rank12Text);
	RankTexts2.Add(Rank22Text);
	RankTexts2.Add(Rank32Text);
	RankTexts2.Add(Rank42Text);
	RankTexts2.Add(Rank52Text);
	RankTexts2.Add(Rank62Text);

	Username1Text = (UTextBlock*)GetWidgetFromName(TEXT("Username1"));
	Username1Text->SetVisibility(ESlateVisibility::Hidden);

	Username2Text = (UTextBlock*)GetWidgetFromName(TEXT("Username2"));
	Username2Text->SetVisibility(ESlateVisibility::Hidden);

	Username3Text = (UTextBlock*)GetWidgetFromName(TEXT("Username3"));
	Username3Text->SetVisibility(ESlateVisibility::Hidden);

	Username4Text = (UTextBlock*)GetWidgetFromName(TEXT("Username4"));
	Username4Text->SetVisibility(ESlateVisibility::Hidden);

	Username5Text = (UTextBlock*)GetWidgetFromName(TEXT("Username5"));
	Username5Text->SetVisibility(ESlateVisibility::Hidden);

	Username6Text = (UTextBlock*)GetWidgetFromName(TEXT("Username6"));
	Username6Text->SetVisibility(ESlateVisibility::Hidden);

	UsernameTexts.Add(Username1Text);
	UsernameTexts.Add(Username2Text);
	UsernameTexts.Add(Username3Text);
	UsernameTexts.Add(Username4Text);
	UsernameTexts.Add(Username5Text);
	UsernameTexts.Add(Username6Text);

	Username11Text = (UTextBlock*)GetWidgetFromName(TEXT("Username11"));
	Username11Text->SetVisibility(ESlateVisibility::Hidden);

	Username21Text = (UTextBlock*)GetWidgetFromName(TEXT("Username21"));
	Username21Text->SetVisibility(ESlateVisibility::Hidden);

	Username31Text = (UTextBlock*)GetWidgetFromName(TEXT("Username31"));
	Username31Text->SetVisibility(ESlateVisibility::Hidden);

	Username41Text = (UTextBlock*)GetWidgetFromName(TEXT("Username41"));
	Username41Text->SetVisibility(ESlateVisibility::Hidden);

	Username51Text = (UTextBlock*)GetWidgetFromName(TEXT("Username51"));
	Username51Text->SetVisibility(ESlateVisibility::Hidden);

	Username61Text = (UTextBlock*)GetWidgetFromName(TEXT("Username61"));
	Username61Text->SetVisibility(ESlateVisibility::Hidden);

	UsernameTexts1.Add(Username11Text);
	UsernameTexts1.Add(Username21Text);
	UsernameTexts1.Add(Username31Text);
	UsernameTexts1.Add(Username41Text);
	UsernameTexts1.Add(Username51Text);
	UsernameTexts1.Add(Username61Text);

	Username12Text = (UTextBlock*)GetWidgetFromName(TEXT("Username12"));
	Username12Text->SetVisibility(ESlateVisibility::Hidden);

	Username22Text = (UTextBlock*)GetWidgetFromName(TEXT("Username22"));
	Username22Text->SetVisibility(ESlateVisibility::Hidden);

	Username32Text = (UTextBlock*)GetWidgetFromName(TEXT("Username32"));
	Username32Text->SetVisibility(ESlateVisibility::Hidden);

	Username42Text = (UTextBlock*)GetWidgetFromName(TEXT("Username42"));
	Username42Text->SetVisibility(ESlateVisibility::Hidden);

	Username52Text = (UTextBlock*)GetWidgetFromName(TEXT("Username52"));
	Username52Text->SetVisibility(ESlateVisibility::Hidden);

	Username62Text = (UTextBlock*)GetWidgetFromName(TEXT("Username62"));
	Username62Text->SetVisibility(ESlateVisibility::Hidden);

	UsernameTexts2.Add(Username12Text);
	UsernameTexts2.Add(Username22Text);
	UsernameTexts2.Add(Username32Text);
	UsernameTexts2.Add(Username42Text);
	UsernameTexts2.Add(Username52Text);
	UsernameTexts2.Add(Username62Text);

}