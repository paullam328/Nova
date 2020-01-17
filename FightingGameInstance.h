// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FightingGameInstance.generated.h"

/**
 * 
 */

//NOTE: THIS GAME INSTANCE IS CORRUPTED

UCLASS()
class NOVAFINAL_API UFightingGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	int32 characterNum = 10;
};
