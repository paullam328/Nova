// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageChangeWidget.generated.h"


class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UStageChangeWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	void DestroyWidget();

	UWidgetAnimation* PopUpDown;

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


public:

	float timer = 0;
	float animLength = 2.0f;
	
};
