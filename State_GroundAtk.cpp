// Copywrite Shapeshifter 2019

#include "State_GroundAtk.h"

State_GroundAtk::State_GroundAtk()
{
	TransitionableStates.push_back(ECharacterStates::AIR);
	TransitionableStates.push_back(ECharacterStates::GROUND);
	TransitionableStates.push_back(ECharacterStates::ATK_GROUND);
	//TransitionableStates.push_back(ECharacterStates::ATK_AIR);
}

/*
Valid states;
 Air,
 Ground,
 Air attack,
 ground attack
*/
bool State_GroundAtk::IsValid(ECharacterStates newState)
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
