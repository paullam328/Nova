// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;

	//UPROPERTY(Replicated)
	float MusSliderVal = 1;

	UPROPERTY(Replicated)
		float SFXSliderVal = 1;

	UPROPERTY(Replicated)
		float VoiceSliderVal = 1;

	void SetMusSliderVal(float Val);

	void SetSFXSliderVal(float Val);

	void SetVoiceSliderVal(float Val);

	UFUNCTION(BlueprintCallable)
		void StoreWwiseDataToGI();
	
};
