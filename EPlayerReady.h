// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EPlayerReady.generated.h"

/**
 *
 */
UENUM()
enum class EPlayerReady
{
	NonExistent = 0,
	NotReady,
	Ready
};
