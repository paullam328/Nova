// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSwitchWipeWidget.generated.h"

class UImage;
class UMaterialInstanceDynamic;
class UTextBlock;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UStageSwitchWipeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UStageSwitchWipeWidget(const FObjectInitializer & ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UMaterialInstanceDynamic* DynMat;
	UImage* Logo;
	FName Disolve = "Disolve";
	UTextBlock* FactText;
	UTextBlock* Text;
	UWidgetAnimation* DissolveInAnim;
	UWidgetAnimation* DissolveOutAnim;
	UWidgetAnimation* FlashingAnim;

	float FlashAnimTimer = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float wipeTime = 0.04;

	bool hasWiped = false;
	bool hasDissolved = false;
	bool hasUndissolved = false;

	UFUNCTION()
	void SetDisolve(float dis);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float testVal = -10;
	
protected:

	virtual void NativeConstruct();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float wipeSpeed = 1;

public:
	UFUNCTION()
	void GenerateFactText();


	bool isTextInitialized = false;

	bool isOnTickWipeActivated = false;

	void Wipe(bool isWipeAppear);

	UFUNCTION()
	void SetTransitionScreenTransparent();


private:
	void PlayDissolveInAnim();

	void PlayFlashingAnim(float DeltaTime);

	void PlayDissolveOutAnim();

};
