// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "WwiseTransitionData.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FWwiseTransitionData
{
	GENERATED_BODY()

	UPROPERTY()
		FString PlayerName;

	UPROPERTY()
		float MusSliderVal;

	UPROPERTY()
		float SFXSliderVal;

	UPROPERTY()
		float VoiceSliderVal;

	//Set
	void SetPlayerName(const FString& pName)
	{
		PlayerName = pName;
	}

	//Get
	FString GetPlayerName()
	{
		return PlayerName;
	}

	//Set
	void SetMusSliderVal(const float mVal)
	{
		MusSliderVal = mVal;
	}

	//Get
	float GetMusSliderVal()
	{
		return MusSliderVal;
	}

	//Set
	void SetSFXSliderVal(const float sVal)
	{
		SFXSliderVal = sVal;
	}

	//Get
	float GetSFXSliderVal()
	{
		return SFXSliderVal;
	}

	//Set
	void SetVoiceSliderVal(const float vVal)
	{
		VoiceSliderVal = vVal;
	}

	//Get
	float GetVoiceSliderVal()
	{
		return VoiceSliderVal;
	}

	//Constructor
	FWwiseTransitionData()
	{
		//Always initialize your USTRUCT variables!
		//exception is if you know the variable type has its own default constructor

		PlayerName = "";
		MusSliderVal = 0;
		SFXSliderVal = 0;
		VoiceSliderVal = 0;
	}
};