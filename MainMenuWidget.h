// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


class UImage;
class UMaterialInstanceDynamic;
class USlider;
class UButton;
class UAkAudioEvent;

UCLASS()
class NOVAFINAL_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UMainMenuWidget(const FObjectInitializer & ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UMaterialInstanceDynamic* DynMat;
	UImage* Logo;
	FName Frame = "Frame";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fps = 0.01;

	USlider* gammaSlider;
	USlider* musicSlider;
	USlider* sfxSlider;
	USlider* voiceSlider;

protected:
	virtual void NativeConstruct();

	UFUNCTION()
		void PlayBTNClick();
	UFUNCTION()
		void OptionsBTNClick();
	UFUNCTION()
		void QuitBTNClick();
	UFUNCTION()
		void CreateGameBTNClick();
	UFUNCTION()
		void FindGameBTNClick();
	UFUNCTION()
		void BackBTNClick();
	UFUNCTION()
		void OptionsBackBTNClick();
	UFUNCTION()
		void gammaSliderValueChanged(float value);
	UFUNCTION()
		void musicSliderValueChanged(float value);
	UFUNCTION()
		void sfxSliderValueChanged(float value);
	UFUNCTION()
		void voiceSliderValueChanged(float value);
	UFUNCTION()
		void FindGameBackBTNClick();
	UFUNCTION()
		void CreateGameBackBTNClick();
	UFUNCTION()
		void SinglePlayerBTNClick();
	UFUNCTION()
		void RefreshBTNClick();
	UFUNCTION()
		void ControllsBTNClick();
	UFUNCTION()
		void ControllsBackBTNClick();
	UFUNCTION()
		void CreditsBTNClick();
	UFUNCTION()
		void CreditsBackBTNClick();

	UFUNCTION(BlueprintCallable, Category = "WwiseAudio")
		void PostWWiseEvent(bool IsHover, bool IsSelect, bool IsBack);
		void ButtonClick(FName thingToHide, FName thingToShow, FName thingToFocus);
	
	UPROPERTY(EditAnywhere, Category = "WwiseAudio")
		UAkAudioEvent* UISelect;
	UPROPERTY(EditAnywhere, Category = "WwiseAudio")
		UAkAudioEvent* UIHover;
	UPROPERTY(EditAnywhere, Category = "WwiseAudio")
		UAkAudioEvent* UIBack;

	UButton* playBTN;
	UButton* optionsBTN;
	UButton* quitBTN;
	UButton* createGameBTN;
	UButton* findGameBTN;
	UButton* backBTN;
	UButton* optionsBackBTN;
	UButton* findGameBackBTN;
	UButton* CreateGameBackBTN;
	UButton* SinglePlayerBTN;
	UButton* RefreshBTN;
	UButton* ControllsBTN;
	UButton* ControllsBackBTN;
	UButton* CreditsBTN;
	UButton* CreditsBackBTN;

	bool playHovered = false;
	bool optionsHovered = false;
	bool quitHovered = false;
	bool createGameHovered = false;
	bool findGameHovered = false;
	bool backHovered = false;
	bool optionsBackHovered = false;
	bool findGameBackHovered = false;
	bool CreateGameBackHovered = false;
	bool SinglePlayerHovered = false;
	bool RefreshHovered = false;
	bool ControllsHovered = false;
	bool ControllsBackHovered = false;

	void HoverWwiseHandler();

	void MainMenu_AdjustSFXSlider(float SFXSliderVal);
};
