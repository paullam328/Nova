// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Condition.h"
#include "WinLoseScreen.generated.h"


class UImage;

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UWinLoseScreen : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UEditableTextBox* WinLoseTextBox;

	UImage* WinImg;
	UImage* LoseImg;


protected:
	// Called when the game starts or when spawned
	virtual void NativeConstruct() override;

public:
	//bool hasWon;

	void SetWinLoseText(ECondition cond);
};
