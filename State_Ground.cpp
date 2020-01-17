// Copywrite Shapeshifter 2019

#include "State_Ground.h"

State_Ground::State_Ground()
{
	TransitionableStates.push_back(ECharacterStates::AIR);
	TransitionableStates.push_back(ECharacterStates::ATK_GROUND);
	TransitionableStates.push_back(ECharacterStates::HIT_GROUND);
}

/*
Valid States,
Air,
Ground_Hit,
Ground_Atk
*/
bool State_Ground::IsValid(ECharacterStates newState)
{
	for (int x = 0; x < TransitionableStates.size(); x++)
	{
		if (TransitionableStates.at(x) == newState)
		{
			return true;
		}
	}
	return false;
}