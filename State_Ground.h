// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "StateInterface.h"

/**
 * 
 */
class NOVAFINAL_API State_Ground : public IStateInterface
{
public:
	State_Ground();

	virtual bool IsValid(ECharacterStates newState) override;

	std::vector<ECharacterStates> TransitionableStates;
};
