// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CharacterSelectPawn.h"
#include "EPlayerReady.h"
#include "CharacterSelectTimerState.h"
#include "CharacterSelectGameState.generated.h"

/**
 * 
 */

class UAkAudioEvent;

UCLASS()
class NOVAFINAL_API ACharacterSelectGameState : public AGameState
{
	GENERATED_BODY()
	
	public:

		ACharacterSelectGameState();

		virtual void BeginPlay() override;

		virtual void Tick(float DeltaTime) override;

public:
		//Setting player indices:--------------------------------------------
		UFUNCTION(Server, WithValidation, Reliable)
			void SetPlayerIndex(); //triggered by each client's button click


		//Setting selected characters:---------------------------------------
		UFUNCTION(Server, WithValidation, Reliable)
			void InitializeCharIndices();

		UFUNCTION(Server, WithValidation, Reliable)
			void SetSelectedCharPerPlayer(int PlayerIndex, ECharacter character);

		//Finalized array: every player's selection is stored here:
		UPROPERTY(Replicated)
			TArray<int> SelectedCharIndices;

		//Checking whether players are ready:---------------------------------
		UFUNCTION(Server, WithValidation, Reliable)
			void InitializePlayersReady();

		UFUNCTION(Server, WithValidation, Reliable)
			void SetPlayerReadyPerPlayer(int PlayerIndex);

		UFUNCTION(Server, WithValidation, Reliable)
			void SetPlayerNotReadyPerPlayer(int PlayerIndex); //On login

		UPROPERTY(Replicated)
		TArray<EPlayerReady> ArePlayersReady;

		UPROPERTY(Replicated)
		int PlayerReadyCounter = 0;

		UPROPERTY(Replicated)
		ECharacterSelectTimerState timerState = ECharacterSelectTimerState::Invalid;

		UPROPERTY(Replicated)
		float timer = 0;

		int mainCountdownMax = 10;
		int getReadyMax = 5;
		int StageWipingMax = 3;
		
		UPROPERTY(EditAnywhere, Category="Dev Mode - Testing Transition Screen")
		bool isTesting = false;
		int testing_MainCountdownMax = 2;
		int testing_GetReadyMax = 2;

		UFUNCTION(Server, WithValidation, Reliable)
		void RunTimer(float DeltaTime);

		UFUNCTION(Server, WithValidation, Reliable)
		void InitializeTimerState(ECharacterSelectTimerState state);

		UFUNCTION(Server, WithValidation, Reliable)
		void Trigger_InstantiateStageSwitchWipeWidget();



//The reworked approach:
		UFUNCTION(Server, WithValidation, Reliable)
		void InsertSelectedCharIntoArray(int32 PlayerIndex, int32 cNum);

		UFUNCTION(Server, WithValidation, Reliable)
		void SetPlayerReadyInArray(int32 PlayerIndex, EPlayerReady IsReady);

		//UFUNCTION(Server, WithValidation, Reliable)
		//void GetSelectedCharFromArray(int32 PlayerIndex, int32& localPlayerSelection);

		UFUNCTION(Server, WithValidation, Reliable)
		void Server_CheckReadyServerTravel();

		void Trigger_CheckReadyServerTravel();

		UFUNCTION(Server, WithValidation, Reliable)
		void AutoLockSelection();

		UPROPERTY(Replicated, EditAnywhere, Category = "Dev Mode - For Solo Server Testing")
		bool isDebugModeActivated = false;

		//CD boot:
		UPROPERTY(Replicated)
			float PerSecondTimer = 0;

		UFUNCTION(NetMulticast, WithValidation, Reliable)
			void CallCountdown(float DeltaTime);

		UFUNCTION(NetMulticast, WithValidation, Reliable)
			void Wwise_CallCountdown(float cdVal, int index);

		UPROPERTY(EditAnywhere, Category = "Wwise")
			UAkAudioEvent* TickTock;

		UPROPERTY(Replicated)
			float IsTransitionSoundPlayed = false;

		UPROPERTY(Replicated)
			TArray<bool> IsPopUpBannerClicked;

		//Setting selected characters:---------------------------------------
		UFUNCTION(Server, WithValidation, Reliable)
			void InitializePopupBannerClicked();

		UFUNCTION(Server, WithValidation, Reliable)
			void SetBannerReadyPerPlayer(int PlayerIndex);

		UFUNCTION(Server, WithValidation, Reliable)
			void SetBannerNotReadyAllPlayers();

		void BlockPlayersFromJoining();

};
