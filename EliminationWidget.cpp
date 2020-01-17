// Copywrite Shapeshifter 2019

#include "EliminationWidget.h"
#include "WidgetAnimation.h"
#include "CanvasPanel.h"
#include "WidgetTree.h"
#include "FighterController.h"

void UEliminationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PopUpDown = GetWidgetTreeOwningClass()->Animations[0];
	PlayAnimation(PopUpDown, 0, 1);

	IsIncremented = false;
}


void UEliminationWidget::DestroyWidget()
{
	if (!IsIncremented)
	{
		AFighterController* FC = Cast<AFighterController>(GetOwningPlayer());
		if (FC != nullptr)
		{
			FC->Client_IncrementNumOfStageCompleteScreensAnimsPlayed();
		}
		IsIncremented = true;
	}
}

void UEliminationWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (this != nullptr)
	{
		if (timer > animLength)
		{
			DestroyWidget();
		}
		else
		{
			timer += DeltaTime;
		}
	}
}