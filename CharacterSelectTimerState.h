// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CharacterSelectTimerState.generated.h"

/**
 * 
 */
UENUM()
enum class ECharacterSelectTimerState
{
	Invalid = 0,
	WaitingForTwoPlayers,
	MainCountdown,
	GetReady,
	StageWiping //before the transition map kicks in!
};
