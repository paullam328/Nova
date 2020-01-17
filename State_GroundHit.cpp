// Copywrite Shapeshifter 2019

#include "State_GroundHit.h"

State_GroundHit::State_GroundHit()
{
	TransitionableStates.push_back(ECharacterStates::GROUND);
	TransitionableStates.push_back(ECharacterStates::HIT_AIR);
}

/*
Valid state;
Air hit
ground
*/
bool State_GroundHit::IsValid(ECharacterStates newState)
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
