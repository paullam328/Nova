// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "LeaderboardData.generated.h"

USTRUCT()
struct FLeaderboardData
{
	GENERATED_BODY()

	UPROPERTY()
		int PlayerIndex;

	UPROPERTY()
		FString PlayerName;

	UPROPERTY()
		int SelectedCharIndex;

	UPROPERTY()
		int Aggression;

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
	void SetSelectedCharIndex(const int32 pSelectedCharIndex)
	{
		SelectedCharIndex = pSelectedCharIndex;
	}

	//Get
	int32 GetSelectedCharIndex()
	{
		return SelectedCharIndex;
	}

	//Set
	void SetAggression(const int32 pAggression)
	{
		Aggression = pAggression;
	}

	//Get
	int32 GetAggression()
	{
		return Aggression;
	}

	//Constructor
	FLeaderboardData()
	{
		//Always initialize your USTRUCT variables!
		//exception is if you know the variable type has its own default constructor

		PlayerName = "";
		PlayerIndex = 0;
		SelectedCharIndex = 0;
		Aggression = 0;
	}
};