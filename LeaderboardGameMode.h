// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerController.h"
#include "LeaderboardGameMode.generated.h"

/**
 * 
 */
class ALeaderboardPawn;

UCLASS()
class NOVAFINAL_API ALeaderboardGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ALeaderboardGameMode();

	// Keeping track of everyplayer that joins in the game...
	TArray<class APlayerController*> PlayerControllerList;

	UFUNCTION()
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION()
	virtual void HandleSeamlessTravelPlayer(AController *& C) override;

protected:
	UPROPERTY(EditAnywhere, Category="Pawn Class")
	TSubclassOf<ALeaderboardPawn> characterToSpawn;
};
