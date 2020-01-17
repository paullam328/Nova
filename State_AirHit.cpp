// Copywrite Shapeshifter 2019

#include "State_AirHit.h"

State_AirHit::State_AirHit()
{
	TransitionableStates.push_back(ECharacterStates::AIR);
	TransitionableStates.push_back(ECharacterStates::HIT_GROUND);
}

/*
Valid States;
GroundHit,
Air
*/
bool State_AirHit::IsValid(ECharacterStates newState)
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
