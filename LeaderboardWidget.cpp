// Copywrite Shapeshifter 2019

#include "LeaderboardWidget.h"
#include "LeaderboardPawn.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Internationalization/Internationalization.h"
#include "Image.h"
#include "WidgetAnimation.h"

void ULeaderboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Rank1Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank1")));
	Rank2Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank2")));
	Rank3Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank3")));
	Rank4Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank4")));
	Rank5Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank5")));
	Rank6Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank6")));
	RankTexts.Add(Rank1Text);
	RankTexts.Add(Rank2Text);
	RankTexts.Add(Rank3Text);
	RankTexts.Add(Rank4Text);
	RankTexts.Add(Rank5Text);
	RankTexts.Add(Rank6Text);

	Rank11Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank11")));
	Rank21Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank21")));
	Rank31Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank31")));
	Rank41Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank41")));
	Rank51Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank51")));
	Rank61Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank61")));
	RankTexts1.Add(Rank11Text);
	RankTexts1.Add(Rank21Text);
	RankTexts1.Add(Rank31Text);
	RankTexts1.Add(Rank41Text);
	RankTexts1.Add(Rank51Text);
	RankTexts1.Add(Rank61Text);

	Rank12Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank12")));
	Rank22Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank22")));
	Rank32Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank32")));
	Rank42Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank42")));
	Rank52Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank52")));
	Rank62Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Rank62")));
	RankTexts2.Add(Rank12Text);
	RankTexts2.Add(Rank22Text);
	RankTexts2.Add(Rank32Text);
	RankTexts2.Add(Rank42Text);
	RankTexts2.Add(Rank52Text);
	RankTexts2.Add(Rank62Text);

	Name1Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name1")));
	Name2Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name2")));
	Name3Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name3")));
	Name4Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name4")));
	Name5Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name5")));
	Name6Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name6")));
	NameTexts.Add(Name1Text);
	NameTexts.Add(Name2Text);
	NameTexts.Add(Name3Text);
	NameTexts.Add(Name4Text);
	NameTexts.Add(Name5Text);
	NameTexts.Add(Name6Text);

	Name11Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name12")));
	Name21Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name22")));
	Name31Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name32")));
	Name41Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name42")));
	Name51Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name52")));
	Name61Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name62")));
	NameTexts1.Add(Name11Text);
	NameTexts1.Add(Name21Text);
	NameTexts1.Add(Name31Text);
	NameTexts1.Add(Name41Text);
	NameTexts1.Add(Name51Text);
	NameTexts1.Add(Name61Text);

	Name12Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name12")));
	Name22Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name22")));
	Name32Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name32")));
	Name42Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name42")));
	Name52Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name52")));
	Name62Text = (UTextBlock*)(WidgetTree->FindWidget(FName("Name62")));
	NameTexts2.Add(Name12Text);
	NameTexts2.Add(Name22Text);
	NameTexts2.Add(Name32Text);
	NameTexts2.Add(Name42Text);
	NameTexts2.Add(Name52Text);
	NameTexts2.Add(Name62Text);


	WinnerNameText = (UTextBlock*)(WidgetTree->FindWidget(FName("WinnerName")));
	WinnerAggressionText = (UTextBlock*)(WidgetTree->FindWidget(FName("WinnerAggression")));

	BackButton = (UButton*)(WidgetTree->FindWidget(FName("BackButton")));
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ULeaderboardWidget::KickMeOut);
		GetOwningPlayer()->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(GetSlateWidgetFromName("BackButton")));
	}

	ColorImg = (UImage*)(WidgetTree->FindWidget(FName("ColorImg")));
	CharImg = (UImage*)(WidgetTree->FindWidget(FName("CharImg")));

	PlayAnimation(GetWidgetTreeOwningClass()->Animations[0], 0, 1);
}

void ULeaderboardWidget::SetLeaderboardText(TArray<FLeaderboardData> data)
{
	int counter = 0;
	int winnerIndex = -1;
	for (int i = data.Num() - 1; i >= 0; i--) //It's a stack
	{
		RankTexts[counter]->SetVisibility(ESlateVisibility::Visible);
		NameTexts[counter]->SetVisibility(ESlateVisibility::Visible);
		RankTexts1[counter]->SetVisibility(ESlateVisibility::Visible);
		NameTexts1[counter]->SetVisibility(ESlateVisibility::Visible);
		RankTexts2[counter]->SetVisibility(ESlateVisibility::Visible);
		NameTexts2[counter]->SetVisibility(ESlateVisibility::Visible);

		FString name = data[i].PlayerName;
		NameTexts[counter]->SetText(FText::FromString(*name));
		NameTexts1[counter]->SetText(FText::FromString(*name));
		NameTexts2[counter]->SetText(FText::FromString(*name));

		if (counter == 0) //Is winner
		{
#define LOCTEXT_NAMESPACE "MyNameSpace"
			WinnerNameText->SetText(FText::Format(NSLOCTEXT("MyNameSpace", "Champ", "CHAMPION: {0}"), FText::FromString(*name)));
#undef LOCTEXT_NAMESPACE

			winnerIndex = i;
		}

		counter++; //to calculate ranking, as this is a stack
	}

	if (data.IsValidIndex(winnerIndex))
	{
		index = data[winnerIndex].PlayerIndex;
		
		//winner index is the index in the array:
		SetLeaderboardChampionImg(data, winnerIndex);
	}
	//CharMesh = Cast<USkeletalMeshComponent>(GetOwningPlayer<APlayerController>()->GetComponentsByTag(USkeletalMesh::StaticClass(), "Char")[0]);
	//Color set FOR NOW based on player index
	//CharMesh->SetMaterial(0, CharMaterials[data[winnerIndex].PlayerIndex]);
	UAkGameplayStatics::SetState("Game_State", "Menu");
	UAkGameplayStatics::PostEvent(BGMusic, GetOwningPlayerPawn());
}

void ULeaderboardWidget::SetLeaderboardChampionImg(TArray<FLeaderboardData> data, int winnerIndex)
{
	for (int i = data.Num() - 1; i >= 0; i--)
	{
		if (data.IsValidIndex(i))
		{
			if (data[i].GetPlayerIndex() == winnerIndex)
			{
				if (CharTextures.IsValidIndex(data[i].GetSelectedCharIndex()) && CharColors.IsValidIndex(data[i].GetPlayerIndex()))
				{
					CharImg->SetBrushFromTexture(CharTextures[data[i].GetSelectedCharIndex()], false);
					ColorImg->SetBrushTintColor(CharColors[data[i].GetPlayerIndex()]);
				}
			}
		}
	}
}

void ULeaderboardWidget::KickMeOut()
{
	ALeaderboardPawn* LP = Cast<ALeaderboardPawn>(GetOwningPlayerPawn());
	if (LP != nullptr)
	{
		LP->Trigger_KickMeOut();
	}
}