// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EliminationWidget.generated.h"

/**
 * 
 */

class UWidgetAnimation;
class UCanvasPanel;

UCLASS()
class NOVAFINAL_API UEliminationWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UWidgetAnimation* PopUpDown;

	bool TriggerAnim = true;

	bool IsLoser = false;

	bool IsIncremented = false;

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


public:

	void DestroyWidget();

	float timer = 0;
	float animLength = 2.0f;
	
};
