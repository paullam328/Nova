// Copywrite Shapeshifter 2019

#include "State_AirAtk.h"

State_AirAtk::State_AirAtk()
{
	TransitionableStates.push_back(ECharacterStates::AIR);
	TransitionableStates.push_back(ECharacterStates::GROUND);
}

/*
Valid States are;
Air,
Ground
*/

bool State_AirAtk::IsValid(ECharacterStates newState)
{
	if (newState == ECharacterStates::AIR)
	{
		return true;
	}
	return false;
}

