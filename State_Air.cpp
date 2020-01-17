// Copywrite Shapeshifter 2019

#include "State_Air.h"

State_Air::State_Air()
{
	TransitionableStates.push_back(ECharacterStates::GROUND);
	TransitionableStates.push_back(ECharacterStates::ATK_AIR);
	TransitionableStates.push_back(ECharacterStates::HIT_AIR);
}


/*
Valid States;
Air hit
Air attakc
Ground
*/
bool State_Air::IsValid(ECharacterStates newState)
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

