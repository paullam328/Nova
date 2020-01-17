// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStateEnum.h"
#include "vector"
#include "StateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NOVAFINAL_API IStateInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual bool IsValid(ECharacterStates newState) = 0;

};
