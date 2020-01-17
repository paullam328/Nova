// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "UnrealNetwork.h"
#include "CharacterPlayerState.h"
#include "Components/TextBlock.h"
#include "ProgressBar.h"

#include "CharacterSelectWidget.generated.h"

/**
 * 
 */
class UAkAudioEvent;
class AAkAmbientSound;
struct FButtonStyle;
class UMaterialInstance;
class UMaterialInstanceDynamic;
class UWidgetAnimation;

class UNetworkingGameInstance; 
UCLASS()
class NOVAFINAL_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UCharacterSelectWidget(const FObjectInitializer & ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CharNum = 0; //pass charNum to maincharacter, then convert it to enum
	
	UButton* Character1Button;
	UImage* Player1SelectImg;
	UImage* Player2SelectImg;
	UImage* Player3SelectImg;
	UImage* Player4SelectImg;
	UImage* Player5SelectImg;
	UImage* Player6SelectImg;
	TArray<UImage*> SelectImgs;

	UTextBlock* Player1Name;
	UTextBlock* Player2Name;
	UTextBlock* Player3Name;
	UTextBlock* Player4Name;
	UTextBlock* Player5Name;
	UTextBlock* Player6Name;
	TArray<UTextBlock*> PlayerNames;

	UTextBlock* Player1ReadyText;
	UTextBlock* Player2ReadyText;
	UTextBlock* Player3ReadyText;
	UTextBlock* Player4ReadyText;
	UTextBlock* Player5ReadyText;
	UTextBlock* Player6ReadyText;
	TArray<UTextBlock*> PlayerReadyTexts;

	UProgressBar* LeaveProgressBar;

	UPROPERTY(EditAnywhere)
		FButtonStyle selectedBorder;
	UPROPERTY(EditAnywhere)
		FButtonStyle staticBorder;

	UPROPERTY(EditAnywhere)
		bool hasSelected;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get has Selected", CompactNodeTitle = "selected", Keyword = "Get has selected"))
		bool GethasSelected() const { return hasSelected; }

	UFUNCTION()
		void changeSelectBorder(int32 cNum);

	UPROPERTY(EditAnywhere, Category = "Characters")
	UTexture2D* DemarkPic;
	UPROPERTY(EditAnywhere, Category = "Characters")
	UTexture2D* MaskiumPic;
	UPROPERTY(EditAnywhere, Category = "Characters")
	UTexture2D* NullPic;

	UImage* TimerImg;
	UTextBlock* TimerText;
	UTextBlock* AlertText;
	UWidgetAnimation* AlertTextAnim;
	UWidgetAnimation* BannerAnim;

	UImage* ReadyBanner;
	UButton* ReadyBannerButton;

	UTextBlock* readyText;

	UFUNCTION()
	void HoldToLeave();

	UFUNCTION()
	void ReleaseLeave();

	UPROPERTY()
	float leaveProgress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float leaveValue = 0.01f;

	UPROPERTY()
	UButton* FocusedWidget = nullptr;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Focused Widget", CompactNodeTitle = "FocusedWidget", Keyword = "Get Focused Widget"))
	UButton* GetFocuedWiget() { return FocusedWidget; }

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Focused Widget", CompactNodeTitle = "FocusedWidget", Keyword = "Set Focused Widget"))
	void SetFocuedWiget(UButton* newWidget) { FocusedWidget = newWidget; }


protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartButton();

	UFUNCTION()
	void setReadyButton();

	UFUNCTION()
		void SelectCharacter(int32 cNum);

	UFUNCTION()
		void SelectCharacterOne();
	UFUNCTION()
		void SelectCharacterTwo();
	UFUNCTION()
		void SelectCharacterThree();
	UFUNCTION()
		void SelectCharacterFour();
	UFUNCTION()
		void SelectCharacterFive();
	UFUNCTION()
		void SelectCharacterSix();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UMaterialInstance* TimerMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UMaterialInstanceDynamic* TimerMatDyn;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent* DemarkQuote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	TSubclassOf<AAkAmbientSound> DemarkQuoteClass;

	AAkAmbientSound* Quote;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent* Quote;

public:
	void SetCharacterImgs();
	void SetReadyTexts();
	void SetAlertText();
	void PlayAnim();
	void PlayReadyBannerAnim();
	UFUNCTION()
	void SetReadyBannerColor_Ready();
	void ReadyPopupBanner();
	void HideBanner();

	void Widget_RemoveFromParent();

private:
	FText SetSecondsText();
	void SetTimeRadian(float DeltaTime);

	UButton* startButton;
	UButton* characterOne;
	UButton* characterTwo;
	UButton* characterThree;
	UButton* characterFour;
	UButton* characterFive;
	UButton* characterSix;

	bool startButtonHovered = false;
	bool characterOneHovered = false;
	bool characterTwoHovered = false;
	bool characterThreeHovered = false;
	bool characterFourHovered = false;
	bool characterFiveHovered = false;
	bool characterSixHovered = false;

	void HoverWwiseHandler();

	UPROPERTY(EditAnywhere, Category = "WwiseAudio")
		UAkAudioEvent* UISelect;
	UPROPERTY(EditAnywhere, Category = "WwiseAudio")
		UAkAudioEvent* UIHover;
	UPROPERTY(EditAnywhere, Category = "WwiseAudio")
		UAkAudioEvent* ReadyBannerVoice;


	UFUNCTION(BlueprintCallable, Category = "WwiseAudio")
		void PostWWiseEvent(bool IsHover, bool IsSelect);

	UPROPERTY(EditAnywhere, Category = "Timer")
	float LerpSpeed = 10;

	void ResetLerpAlphaOnStage();
	bool IsGetReadyAlphaReset = false;
	bool IsStageSwipeAlphaReset = false;

public:
	UPROPERTY(BlueprintReadWrite, Category = "TestingInput")
	bool IsBannerReady = false;

	void StartButton_SetColor(bool IsReady);

	void Widget_SetUserFocus(FString WidgetToFocusStr, APlayerController* PC);

	void Widget_OnLeaveReadySetFocus(APlayerController* PC);

	float LerpAlpha = 0;
	float LerpStart = 0;
	float LerpTarget = 1;

	int maxSec = 10;
	bool ShouldShowTimer = false;

	float Widget_timerSec = 0;

	//Extra stuff:
	void SetSelfNameGreen(int i);

	UPROPERTY(EditAnywhere, Category = "WwiseAudio")
		UAkAudioEvent* UIReady;
};
