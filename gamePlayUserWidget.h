// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "gamePlayUserWidget.generated.h"


class USlider;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UgamePlayUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UgamePlayUserWidget(const FObjectInitializer & ObjectInitializer);
	virtual void Tick(float InDeltaTime, FGeometry MyGeometry);

	USlider* gammaSlider;
	UImage* steamImage;
	UTextBlock* steamName;


	UFUNCTION()
		void resumeButton();
	UFUNCTION()
		void quitButton();
	UFUNCTION()
		void optionsButton();
	UFUNCTION()
		void optionsBackButton();
	UFUNCTION()
		void gammaSliderValueChanged(float value);


	// VV Advanced Sesions Things VV //

	/*static bool HasOnlineSubsystem(FName SubSystemName);*/

protected:

	virtual void NativeConstruct();

	UFUNCTION()
		void ButtonClick(FName thingToHide, FName thingToShow, FName thingToFocus);


};
