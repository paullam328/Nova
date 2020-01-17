// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "StateInterface.h"
/**
 * 
 */
class NOVAFINAL_API State_AirAtk : public IStateInterface
{
public:
	State_AirAtk();

	virtual bool IsValid(ECharacterStates newState) override;

	std::vector<ECharacterStates> TransitionableStates;
};
