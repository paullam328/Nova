// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UnrealNetwork.h"
#include "MainMenuGameState.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API AMainMenuGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UFUNCTION(Server, WithValidation, Reliable)
	void UnpossessPawns();

};

