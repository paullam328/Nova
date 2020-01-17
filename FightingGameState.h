// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "UnrealNetwork.h"
#include "Condition.h"
#include "FinalStageData.h"
#include "LeaderboardData.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"

#include "FightingGameState.generated.h"

/**
 * 
 */

class AStageTransitionManager;
class AFighterBase;
class APlayerController;

class UAkAudioEvent;
class ALevelMusicManager;
class UAkComponent;

class AFighterSpectator;

UENUM()
enum class ETimerState : uint8
{
	Invalid = 0,
	Initialization, //gotta initialize the game data, spawn the right character, etc.,
	SetPawnPosition, //Properly set the pawns into their desired positions, after they are initialized
	EndTransition, // gotta end the transition
	EndTransitionCompleted, // end transition completed
	InitialCountdown,
	MainCountdown,
	DeadPlayerElimination,
	Elevator_TransitionIn,
	Elevator,
	Elevator_TransitionOut,
	FinalStage, //timer is disabled in final stage
	WinnerDecided, //3 2 1 kickplayers
	ServerTravelling
};

UCLASS()
class NOVAFINAL_API AFightingGameState : public AGameState
{
	GENERATED_BODY()

	public:
		//constructor:
		AFightingGameState();

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		UPROPERTY(EditAnywhere, Category = "Stage Transition Manager")
		TSubclassOf<AStageTransitionManager> StageTransitionManagerClass;
	
		virtual void Tick(float DeltaTime) override;

		

	private:

		float timer2 = 0;

//Timer:=======================================================================

	static const int initialTimerMax = 4;

	const float transitionInSlightDelayMax = 0.5f;

	//For testing:
	UPROPERTY(EditAnywhere, Category = "Dev Mode - Do not run dedicated server")
	int mainTimerMax = 60;

	UPROPERTY(EditAnywhere, Category = "Dev Mode - Do not run dedicated server")
	float deadPlayerEliminationTimerMax = 3.0f;
	
public:

	UFUNCTION(Server, WithValidation, Reliable)
	void RunTimer(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float timer = 4.0f;

	UPROPERTY(BlueprintReadWrite, Replicated)
	ETimerState timerState;


	//Outdated functions:
	void RunWinLoseScreen();

//Stage Transitions:===============================================================

	UFUNCTION(Server, WithValidation, Reliable)
	void SetStartingPoints();
	UFUNCTION(Server, WithValidation, Reliable)
	void TeleportPlayersToStage(int stageNum, bool isJustStarted);
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void TogglePawnInput(bool setEnabled);



	UPROPERTY(Replicated)
	TArray<AActor*> Stage2StartingPoints;

	UPROPERTY(Replicated)
	TArray<AActor*> Stage3StartingPoints;

	UPROPERTY(Replicated)
	TArray<AActor*> Stage4StartingPoints;

	UPROPERTY(Replicated)
	TArray<AActor*> Stage5StartingPoints;

	UPROPERTY(Replicated)
	TArray<AActor*> StageEStartingPoints;

	UPROPERTY(Replicated)
	AFighterBase* awaitingToDelete;

	UPROPERTY(Replicated)
	int currentStage = 1;

	UPROPERTY(Replicated)
	int currentStageIndex;

	UPROPERTY(Replicated)
	bool hasBeenInElevator = false;
	UPROPERTY(Replicated)
	bool hasGotOffElevator = false;

//Stage Transitions (Player Elimination):========================================

	UFUNCTION(Server, WithValidation, Reliable)
	void AddToSurvivingPlayers();

	UFUNCTION(Server, WithValidation, Reliable)
	void EliminatePlayerNormalStage();

	UFUNCTION(Server, WithValidation, Reliable)
	void StartGame();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Trigger_InitializeMusicSlider(int stageNum);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SpawnLevelMusicManager();

	UPROPERTY(Replicated)
	bool hasGameStarted = false;

	UPROPERTY(Replicated)
	int numOfSurvivingPlayers = 6; //have to set this; has to be set in the selection screen (assume this is set)

	UPROPERTY(Replicated)
	TArray<AFighterBase*> survivingPlayers;

	UPROPERTY(Replicated, EditAnywhere, Category="Dev Mode - Do not run dedicated server")
	int numOfSelectionScreenPlayers = 2; 

//Dead Spectators:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dead Spectators")
	TSubclassOf<AFighterSpectator> FighterSpectatorClass;

	//Music Manager

//Retrieve Player Data on Game Start:==============================================
	
	UPROPERTY(EditAnywhere, Category = "Character")
	TSubclassOf<AFighterBase> DemarkClass;

	UPROPERTY(EditAnywhere, Category = "Character")
	TSubclassOf<AFighterBase> MaskiumClass;

	UPROPERTY(Replicated)
	bool isNumPlayerSet = false;

	UFUNCTION(Server, WithValidation, Reliable)
	void RetrieveTransitionData();

	UPROPERTY(Replicated)
	TArray<AActor*> Stage1StartingPoints;

	UPROPERTY(Replicated)
	bool isGameInitialized = false;

	UPROPERTY(Replicated)
	int isCharSetCounter = 0;

//Tick State Manager:
	UFUNCTION(Server, WithValidation, Reliable)
	void TickBasedOnState();

//For developer game mode:
	//DO NOT check this box if you want a complete game loop from the selection screen
	UPROPERTY(Replicated, EditAnywhere, Category = "Dev Mode - Do not run dedicated server")
	bool activateDevMode = false;

	//Set 0 for Demark, 1 for Maskium
	UPROPERTY(Replicated, EditAnywhere, Category = "Dev Mode - Do not run dedicated server")
	int charToTest = 0;

	UPROPERTY(Replicated, EditAnywhere, Category = "Dev Mode - Do not run dedicated server")
	int32 howLongYouStayInElevator = 600;

//New approach to decide the winner:
	UPROPERTY(Replicated)
	TArray<FFinalStageData> FinalStageDataArr;

	UFUNCTION(Server, WithValidation, Reliable)
	void InitializeFinalStageData();

	//Called by player who's 0 health:
	UFUNCTION(Server, WithValidation, Reliable)
	void OnZeroHealthDecideWinLose(AFighterBase* fb);

//Edge cases: =======================================

	// 2 players:
	UPROPERTY(Replicated)
	TArray<AFighterBase*> SpawnedPawns;

//On Winner Decided:
	UFUNCTION(Server, WithValidation, Reliable)
	void TeleportPlayersToLeaderboardMap();

	UPROPERTY(Replicated, EditAnywhere)
	TArray<UMaterialInterface*> CharColour;

//Create Leaderboard Data:
	UFUNCTION(Server, WithValidation, Reliable)
	void PushStoreLeaderboardData(AFighterBase* fighter);

	UPROPERTY(Replicated)
		AFighterBase* WinningPlayer;

//Wwise:
	UFUNCTION(Server, WithValidation, Reliable)
	void CallCountdown(float DeltaTime);

	UPROPERTY(Replicated)
	float PerSecondTimer = 0.97f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * initalAnnouncer;

	UPROPERTY(EditAnywhere, Category = "Wwise")
	UAkAudioEvent* announcer;

	UPROPERTY(EditAnywhere, Category = "Wwise")
	UAkAudioEvent* StageEvent;

	UPROPERTY(EditAnywhere, Category = "Wwise")
	UAkAudioEvent* Transition;

	UPROPERTY(Replicated, EditAnywhere, Category = "Wwise")
		float testVol = 0.0f;



//SetPawnPosition:
	UFUNCTION(Server, WithValidation, Reliable)
	void SetPawnPos();

//Transition:
	UFUNCTION(Server, WithValidation, Reliable)
	void EndTransition();

	UPROPERTY(Replicated)
	bool bTransitionFullyDisappeared = false;
	UPROPERTY(Replicated)
		bool bIsBackgroundMusicPlayed = false;

	void AdjustVoiceSlider();

	//Each stage, there's a loser...
	//Reset this after the loser is eliminated
	//AKA. first one to be 0 health...
	//If loser is not null, then don't need to assign another one

	UPROPERTY(Replicated)
	AFighterBase* Loser;

	UFUNCTION(Server, WithValidation, Reliable)
	void DetectAssignLoser();

	UFUNCTION(Server, WithValidation, Reliable)
	void SurvivingPlayers_SetFullHealth();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void StageMusicTransition();

	//Elevator wipe:========================================================
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void ElevatorWipe(ETimerState nextState, float DeltaTime);

	UPROPERTY(Replicated)
	float ElevatorWipeStopTime = 2;

	UPROPERTY(Replicated)
	bool ElevatorWipe_HasWipeInstantiatedFadedIn = false;

	UPROPERTY(Replicated)
	bool ElevatorWipe_IsDelayFinished = false;
	
	UPROPERTY(Replicated)
	bool ElevatorWipe_HasWipeStopFinished = false;


	UPROPERTY(Replicated)
	bool HasStageChangeUIInstantiated = false;

	UPROPERTY(Replicated)
	bool isDevModeScreenWipeDisabled = false;

	UFUNCTION(Server, WithValidation, Reliable)
	void ExecuteDeadPlayerEliminationSequence();
	
	UFUNCTION(Server, WithValidation, Reliable)
	void InstantiateDeadPlayerUI();

	UFUNCTION(Server, WithValidation, Reliable)
	void InstantiateStageChangeUI();

	UFUNCTION(Server, WithValidation, Reliable)
	void DeletePlayer();

	UFUNCTION(Server, WithValidation, Reliable)
	void PossessSpectatorPawnAndDeletePlayer();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void PostTimerSound(float cdVal);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void PostInitialTimerSound(float cdVal);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void PostGameOverSound();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void PostCharDieEvent();

	UPROPERTY(Replicated)
	bool IsCharDieSoundPlayed;

	//Try to spawn the level music manager here instead...
	UPROPERTY(Replicated)
	ALevelMusicManager* LevelMusicManager;

	UPROPERTY(EditAnywhere, Category="Wwise")
	TSubclassOf<ALevelMusicManager> LevelMusicManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firework")
	TSubclassOf<AActor> FireworkBP;

	UPROPERTY(Replicated)
	bool IsThirtySecRTPCSet = false;

	UPROPERTY(Replicated)
	bool IsZeroSecRTPCSet = false;

	//just for wwise...
	UPROPERTY(Replicated)
	int Wwise_PlayerRanking = -1;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetCountdownRankingState(float cdtimer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * ElevatorTransitionEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * CharDieEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * gameOverSound;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void PlayElevatorTransitionSound();

	UPROPERTY(Replicated)
	int numOfStageCompleteScreensInstantiated = 0;

	UPROPERTY(Replicated)
	int numOfStageCompleteScreensAnimsPlayed = 0;

	UFUNCTION(Server, WithValidation, Reliable)
	void Trigger_InstantiateSpectatorWidget();

	UPROPERTY(Replicated)
	bool IsLoserDetected = false;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void PlayRoofMusic();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SpawnFirework();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetMusicStateOnNumOfPlayers();

	UFUNCTION(Server, WithValidation, Reliable)
	void Trigger_StopAllAnims();
	
	UAkComponent * BackgroundMusicComp;

	void AdjustBackgroundMusicCompVol();

	float WidgetSpawnTime = 500;
};
