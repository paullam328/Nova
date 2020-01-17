// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CharacterStateEnum.generated.h"

/**
 * 
 */
UENUM()
enum class ECharacterStates
{
	GROUND = 0,
	AIR,
	ATK_GROUND,
	ATK_AIR,
	HIT_GROUND,
	HIT_AIR

};

/*
UCLASS()
class NOVAFINAL_API UCharacterStateEnum : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
*/
