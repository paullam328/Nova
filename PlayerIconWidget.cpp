// Copywrite Shapeshifter 2019

#include "PlayerIconWidget.h"
#include "Engine.h"
#include "Image.h"
#include "HorizontalBox.h"
#include "TextBlock.h"

UPlayerIconWidget::UPlayerIconWidget(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UPlayerIconWidget::SetPlayerImage(int cNum)
{
	if (hasUpdatedIcon == false)
	{
		UImage* playerPic = (UImage*)GetWidgetFromName(TEXT("PlayerIcon"));
		if (cNum == 1)
		{
			//Demark
			playerPic->SetBrush(demarkImage);
			hasUpdatedIcon = true;
		}
		else if (cNum == 0)
		{
			playerPic->SetBrush(maskiumImage);
			hasUpdatedIcon = true;
		}
	}
}

void UPlayerIconWidget::SetArrowVisibility(int index)
{
	if (!hasUpdatedArrow)
	{
		if (ColorArrows.IsValidIndex(index))
		{
			UImage* YouArrow = (UImage*)GetWidgetFromName(TEXT("YouArrow"));
			YouArrow->SetVisibility(ESlateVisibility::Visible);
			YouArrow->SetBrushFromTexture(ColorArrows[index], false);
			hasUpdatedArrow = true;
		}
	}
}

void UPlayerIconWidget::SetImageColor(int index)
{
	if (!hasUpdatedImageColor)
	{
		UImage* PlayerIconBackGround = (UImage*)GetWidgetFromName(TEXT("PlayerIconBackGround"));
		if (CharColors.IsValidIndex(index))
		{
			PlayerIconBackGround->SetBrushTintColor(CharColors[index]);
		}
		hasUpdatedImageColor = true;
	}
}

void UPlayerIconWidget::SetName(FText Name)
{
	if (!hasUpdatedName)
	{

		UTextBlock* Text1 = (UTextBlock*)GetWidgetFromName(TEXT("PlayerName1"));
		UTextBlock* Text2 = (UTextBlock*)GetWidgetFromName(TEXT("PlayerName2"));
		UTextBlock* Text3 = (UTextBlock*)GetWidgetFromName(TEXT("PlayerName3"));
		Text1->SetText(Name);
		Text2->SetText(Name);
		Text3->SetText(Name);

		hasUpdatedName = true;
	}
}

void UPlayerIconWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
