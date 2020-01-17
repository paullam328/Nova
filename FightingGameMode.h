// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/Pawn.h"
#include "FighterBase.h"
#include "UnrealNetwork.h"
#include "FightingGameMode.generated.h"

class UGameplayStatics;
class AFighterBase;
class UNetworkingGameInstance;
class UAkAudioEvent;
/**
 * 
 */
UCLASS()
class NOVAFINAL_API AFightingGameMode : public AGameMode
{
	GENERATED_BODY()


public:

	AFightingGameMode();

	void SetCharNum(int32 val);
	// Keeping track of everyplayer that joins in the game...
	TArray<class APlayerController*> PlayerControllerList;

private:


	
	// ------------------- Variables -------------------

	// Maximum Players that are needed/ allowed during a match...
	int32 MaxNumPlayers;

	int32 charNum = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFighterBase> Demark;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFighterBase> Maskium;

	//------------------ Functions ------------------
	

	UFUNCTION()
	virtual void Logout(AController* Exiting) override;


	// Overloading the ReadyToStartMatch Function to keep only start a game when there is enough players
	UFUNCTION()
	virtual bool ReadyToStartMatch_Implementation() override;

	// Pre Login Level 

	FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

	UFUNCTION()
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	UFUNCTION()
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	// Post Login Function adds new player everytime they join the game
	UFUNCTION()
	virtual void PostLogin(APlayerController* NewPlayer) override;

	//For Seamless Travel, PostLogin doesn't get called, so we use this function instead:
	virtual void HandleSeamlessTravelPlayer(AController *& C) override;
	

	//void HandleStartingNewPlayer(APlayerController * NewPlayer) override;


	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	// Internal Timer...
	//UFUNCTION()
	//void RunTimer(float DeltaTime);

	// InternalTimer
	//float InternalTimer = 0.f;


	// Maintaining the GameLoop from Server Side...
	//UFUNCTION()
	//void ResetIfNoPlayers();

public: 
	// Creating a getter function that will return the amount of players that will be in the game
	UFUNCTION()
	TArray<class APlayerController*> PlayerArray();



	// This function must be called to set max amount of players before the stage map is ready.
	void SetMaxNumPlayers(int32 NumOfPlayerReady);

	UPROPERTY(Replicated)
	int isCharSetCounter = 0;

	UPROPERTY(Replicated)
	bool isEveryPlayerLoaded = false;

	UPROPERTY(Replicated, EditAnywhere, Category = "Dev Mode - Do not run dedicated server")
	int numOfSelectionScreenPlayers = -1;

	UFUNCTION(Server, WithValidation, Reliable)
	void IncrementIsCharSetCounterAndCheckIfAllReady();

	UFUNCTION(Server, WithValidation, Reliable)
	void SetNumOfSelectionScreenPlayers(int num);

	UFUNCTION(Server, WithValidation, Reliable)
	void Trigger_ServerTravel();

	UFUNCTION(Server, WithValidation, Reliable)
	void RetrieveWwiseData(UNetworkingGameInstance* GI);

	//Test Wwise approach:

	UPROPERTY(EditAnywhere)
	UAkAudioEvent* Stage1Event;

};
