// Copywrite Shapeshifter 2019

#pragma once


#include "CoreMinimal.h"
#include "FighterPlayerState.h"
#include "MapTransitionData.generated.h"


USTRUCT()
struct FMapTransitionData
{
	GENERATED_BODY()

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int PlayerIndex;

	UPROPERTY()
	int SelectedCharIndex;

	//Set
	void SetPlayerName(const FString& pName)
	{
		PlayerName = pName;
	}

	//Get
	FString GetPlayerState()
	{
		return PlayerName;
	}

	//Set
	void SetPlayerIndex(const int32 pIndex)
	{
		PlayerIndex = pIndex;
	}

	//Get
	int32 GetPlayerIndex()
	{
		return PlayerIndex;
	}

	//Set
	void SetSelectedCharIndex(const int32 pSelectedCharIndex)
	{
		SelectedCharIndex = pSelectedCharIndex;
	}

	//Get
	int32 GetSelectedCharIndex()
	{
		return SelectedCharIndex;
	}

	//Constructor
	FMapTransitionData()
	{
		//Always initialize your USTRUCT variables!
		//exception is if you know the variable type has its own default constructor
		
		PlayerName = "";
		PlayerIndex = 0;
		SelectedCharIndex = 0;
	}
};