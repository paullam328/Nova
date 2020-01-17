// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "WidgetTree.h"
#include "LeaderboardData.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Styling/SlateColor.h"
#include "LeaderboardWidget.generated.h"

/**
 * 
 */
class UAkAudioEvent;
class UImage;

UCLASS()
class NOVAFINAL_API ULeaderboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UTextBlock* Rank1Text;
	UTextBlock* Rank2Text;
	UTextBlock* Rank3Text;
	UTextBlock* Rank4Text;
	UTextBlock* Rank5Text;
	UTextBlock* Rank6Text;
	TArray<UTextBlock*> RankTexts;

	UTextBlock* Rank11Text;
	UTextBlock* Rank21Text;
	UTextBlock* Rank31Text;
	UTextBlock* Rank41Text;
	UTextBlock* Rank51Text;
	UTextBlock* Rank61Text;
	TArray<UTextBlock*> RankTexts1;

	UTextBlock* Rank12Text;
	UTextBlock* Rank22Text;
	UTextBlock* Rank32Text;
	UTextBlock* Rank42Text;
	UTextBlock* Rank52Text;
	UTextBlock* Rank62Text;
	TArray<UTextBlock*> RankTexts2;

	UTextBlock* Name1Text;
	UTextBlock* Name2Text;
	UTextBlock* Name3Text;
	UTextBlock* Name4Text;
	UTextBlock* Name5Text;
	UTextBlock* Name6Text;
	TArray<UTextBlock*> NameTexts;

	UTextBlock* Name11Text;
	UTextBlock* Name21Text;
	UTextBlock* Name31Text;
	UTextBlock* Name41Text;
	UTextBlock* Name51Text;
	UTextBlock* Name61Text;
	TArray<UTextBlock*> NameTexts1;

	UTextBlock* Name12Text;
	UTextBlock* Name22Text;
	UTextBlock* Name32Text;
	UTextBlock* Name42Text;
	UTextBlock* Name52Text;
	UTextBlock* Name62Text;
	TArray<UTextBlock*> NameTexts2;

	UTextBlock* WinnerNameText;
	UTextBlock* WinnerAggressionText;

	UButton* BackButton;

	UImage* CharImg;
	UImage* ColorImg;

	int index = -1;
	int winnerIndex = -1;
	bool isMaterialSet = false;

protected:
	virtual void NativeConstruct();

	UFUNCTION()
	void KickMeOut();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imgs")
	TArray<UTexture2D*> CharTextures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imgs")
	TArray<FSlateColor> CharColors;

public:
	void SetLeaderboardText(TArray<FLeaderboardData> data);

	void SetLeaderboardChampionImg(TArray<FLeaderboardData> data, int winnerIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent* BGMusic;

	float destroyUITimer = 0;
};
