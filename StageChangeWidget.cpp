// Copywrite Shapeshifter 2019

#include "StageChangeWidget.h"
#include "WidgetAnimation.h"
#include "WidgetTree.h"

void UStageChangeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PopUpDown = GetWidgetTreeOwningClass()->Animations[0];
	PlayAnimation(PopUpDown, 0, 1);
}

void UStageChangeWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
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

void UStageChangeWidget::DestroyWidget()
{
	RemoveFromParent();
}
