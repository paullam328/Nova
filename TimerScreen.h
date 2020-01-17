// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "FightingGameState.h"
#include "TimerScreen.generated.h"

/**
 * 
 */

class UWidgetAnimation;
class UTextBlock;
class UImage;

UCLASS()
class NOVAFINAL_API UTimerScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void BP_Tick();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	FText SetMinutesText();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	FText SetSecondsText();

	void SetInitialCDText();



	UFUNCTION(BlueprintCallable, Category = "Timer")
	ESlateVisibility SetThreeVisibility();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	ESlateVisibility SetTwoVisibility();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	ESlateVisibility SetOneVisibility();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	ESlateVisibility SetGoVisibility();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	ESlateVisibility SetMainCDVisibility();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	ESlateVisibility SetInitialCDVisibility();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	ESlateVisibility SetFinalVisibility();

	UEditableTextBox* MinutesTextBox;
	UEditableTextBox* SecondsTextBox;

	UImage* Three;
	UImage* Two;
	UImage* One;
	UImage* Go;

	UWidgetAnimation* Swipe_Left; //3
	UWidgetAnimation* Swipe_Right; //2
	UWidgetAnimation* Swipe_Top; //1
	UWidgetAnimation* Size; //Fight

	bool isThreeAnimPlayed = false;
	bool isTwoAnimPlayed = false;
	bool isOneAnimPlayed = false;
	bool isGoAnimPlayed = false;

public:
	//Client val store:
	float Timer = 0;
	ETimerState TimerState = ETimerState::Invalid;
	
};
