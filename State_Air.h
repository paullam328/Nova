// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "StateInterface.h"

/**
 * 
 */
class NOVAFINAL_API State_Air : public IStateInterface
{
public:
	State_Air();

	virtual bool IsValid(ECharacterStates newState) override;

	std::vector<ECharacterStates> TransitionableStates;
};
