// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Condition.h"
#include "FinalStageData.generated.h"


USTRUCT()
struct FFinalStageData
{
	GENERATED_BODY()

	UPROPERTY()
	int PlayerIndex;

	UPROPERTY()
	ECondition Condition;

	//Set
	void SetPlayerIndex(const int32 pIndex)
	{
		PlayerIndex = pIndex;
	}

	//Set
	void SetCondition(const ECondition cond)
	{
		Condition = cond;
	}

	//Get
	int32 GetPlayerIndex()
	{
		return PlayerIndex;
	}

	//Get
	ECondition GetCondition()
	{
		return Condition;
	}

	//Constructor
	FFinalStageData()
	{
		//Always initialize your USTRUCT variables!
		//exception is if you know the variable type has its own default constructor

		PlayerIndex = 0;
		Condition = ECondition::Undecided;
	}
};