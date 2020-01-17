// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateColor.h"
#include "PlayerIconWidget.generated.h"


class UImage;

struct FSlateBrush;
/**
 * 
 */
UCLASS()
class NOVAFINAL_API UPlayerIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPlayerIconWidget(const FObjectInitializer & ObjectInitializer);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Aggression", CompactNodeTitle = "Aggression", Keyword = "Get Aggression ammount"))
		float GetAggression() const { return agg; }
	UFUNCTION()
		void SetAggression(float newAgg) { agg = newAgg; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get HP", CompactNodeTitle = "HP", Keyword = "Get HP ammount"))
		float GetHealth() const { return hp; }
	UFUNCTION()
		void SetHP(float newHP) { hp = newHP; }

	UFUNCTION()
		void SetComboNumber(float newComboNumber) { comboNumber = newComboNumber; }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get ComboNumber", CompactNodeTitle = "Combo Number", Keyword = "Get Combo Number"))
		int GetComboNumber() const { return comboNumber; }

	UFUNCTION()
	void SetPlayerImage(int cNum);

protected:

	virtual void NativeConstruct();
	UPROPERTY(EditAnywhere)
		float agg = 0.0f;
	UPROPERTY(EditAnywhere)
		float hp = 100.0f;
	UPROPERTY(EditAnywhere)
		int comboNumber = 0;
	UPROPERTY(EditAnyWhere)
		int cNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSlateBrush demarkImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSlateBrush maskiumImage;


public:
	//Implement color and scale:

	UFUNCTION()
	void SetImageColor(int index);

	UFUNCTION()
	void SetArrowVisibility(int index);

	UFUNCTION()
	void SetName(FText Name);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imgs")
	TArray<FSlateColor> CharColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imgs")
	TArray<UTexture2D*> ColorArrows;


	UPROPERTY()
		bool hasUpdatedIcon = false;

	UPROPERTY()
		bool hasUpdatedArrow = false;

	UPROPERTY()
		bool hasUpdatedImageColor = false;

	UPROPERTY()
		bool hasUpdatedName = false;
};
