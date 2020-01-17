// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EPlayerReady.h"
#include "UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "CharacterSelectionGameMode.generated.h"

/**
 * 
 */

class UCharacterSelectWidget;
UCLASS()
class NOVAFINAL_API ACharacterSelectionGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ACharacterSelectionGameMode();
	
	UFUNCTION()
	virtual void PostLogin(APlayerController* NewPlayer) override;


	UFUNCTION()
	virtual void Logout(AController* Exiting) override;

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_ServerTravel();



	UFUNCTION(BlueprintCallable)
	void RegisterServerSteam();

	// Steam Authentication, only true if its not the editor...
	bool bUseAuthentication = false;

	// Keeping track of everyplayer that joins in the game...
	TArray<APlayerController*> PlayerControllerList;

	/*UFUNCTION(Server, WithValidation, Reliable)
	void SetPlayerIndex(APlayerController* NewPlayer);*/

};
