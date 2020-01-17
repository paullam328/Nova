// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "FighterPlayerState.h"
#include "UnrealNetwork.h"
#include "FightingGameState.h"
#include "gamePlayUserHUD.h"
#include "FighterController.generated.h"

class UStageSwitchWipeWidget;
class UMiniLeaderboardWidget;
class UAkAudioEvent;
class UTimerScreen;
class UPlayerIconWidget;
class AgamePlayUserHUD;
class UgamePlayUserWidget;
class UUserWidget;
class UEliminationWidget;
class UStageChangeWidget;


/**
 * 
 */
UCLASS()
class NOVAFINAL_API AFighterController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AFighterController();

	// Last time the client's ping was updated
	UPROPERTY()
	float LastPingUpdateTime;


	// Setting the Update Interval for Ping
	UPROPERTY()
	float PingUpdateInteral = 0.f;

	// Max amount of ping to predict ahead..
	const float MaxPredictionPing = 255.0f;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void MakeUserHudLarge(UPlayerIconWidget* icon);
	// Used to correct prediction Error.
	// Need to correct this, its important for the
	// getting accurate lag compensation.

	const float PredicitionCorrectionFactor = 1.8f;
	// =============================================== //

	// Runs on client, checks where or nnot we should send a ping to the server
	UFUNCTION()
	void CheckSendPing();

	// Returns amount of time, in seconds, to tick to make up for network lag..
	virtual float GetPredictionTime();


	// Request a ping update. called on Client
	UFUNCTION(Unreliable, Server, WithValidation)
	virtual void ServerBouncePing(float TimeStamp);


	// Respond to a client-requested ping update
	UFUNCTION(Unreliable, Client)
	virtual void ClientReturnPing(float TimeStamp);

	// Client informs server of new ping
	UFUNCTION(Unreliable, Server, WithValidation)
	virtual void ServerUpdatePing(float TimeStamp);

	UFUNCTION()
	virtual void PlayerTick(float DeltaTime);

	UFUNCTION()
	virtual void SetupInputComponent() override;

	void Pause();

	UPROPERTY(BlueprintReadOnly)
	AFighterPlayerState* MPlayerState;

	void InitPlayerState();

	virtual void OnRep_PlayerState();

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_InstantiateStageSwitchWipeWidget(bool isWipeAppear);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InstantiateStageSwitchWipeWidget(bool isWipeAppear, bool isFirstWipe);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void WipeTransitionScreen(bool isWipeAppear);

	UPROPERTY(Replicated)
	bool hasWiped = false;

	UPROPERTY(Replicated)
	bool isStageSwitchWipeWidgetNull = true;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetHasWiped();

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SetStageSwitchWipeWidgetNull(bool isNull);

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SetHasWiped(bool wiped);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void DestroyWipeWidget();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Trigger_WidgetOnTickWipe();

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_WidgetOnTickWipe();

	//Check if I'm a spectator
	UPROPERTY(Replicated)
	bool isSpectating = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
	TSubclassOf<UStageSwitchWipeWidget> StageSwitchWipeWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
	TSubclassOf<UMiniLeaderboardWidget> MiniLeaderboardWidgetClass;

public:
	UStageSwitchWipeWidget* StageSwitchWipeWidget;

	UMiniLeaderboardWidget* MiniLeaderboardWidget;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InstantiateMiniLeaderboardWidget();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void DestroyMiniLeaderboardWidget();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Initialize_RemoveAllWidgets();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SpawnGameplayWidgets();


public:

	AgamePlayUserHUD* HUD;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerIconWidget> iconWidget;
	UPROPERTY()
	UPlayerIconWidget* icon;

	UPROPERTY()
	UPlayerIconWidget* yourIcon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WidgetClass;
	UUserWidget* widget;

	bool isPause = false;

	UFUNCTION()
	void RunGamePlayWidget();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void UpdatePlayerHealth();

	UPROPERTY()
	TArray<AActor*> allFighters;

	UPROPERTY()
	TArray<UPlayerIconWidget*> allIcons;

	UPROPERTY(Replicated)
	bool hasUI = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game starts or when spawned
	virtual void AcknowledgePossession(APawn* P) override;

	// Called when the game starts or when spawned
	virtual void PreInitializeComponents() override;

public:
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Multicast_GetSurvivingPlayers();

	//UFUNCTION(Server, WithValidation, Reliable)
	//void Server_GetPlayerArray(const TArray<APlayerController*>& ary);

	UPROPERTY(Replicated)
	TArray<AFighterBase*> FighterBases;

	//For Wwise get player name:
	UPROPERTY(Replicated)
	FString UI_PlayerName = "";

	void Trigger_GetPlayerName();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Server_GetPlayerName();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void PlayTransitionSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * TransitionEvent;

	//==========Paul's Win/Lose UI Stuff================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
		TSubclassOf<UTimerScreen> TimerScreenClass;

	UTimerScreen* TimerScreen;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Debug_RemoveAllWidgets();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent * ElevatorTransitionEvent;

	UFUNCTION(Client, WithValidation, Reliable)
	void Multicast_SetTransitionScreenTransparent();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sorry, just to minimize the chances of crash happenining...")
	bool IsDevMode = false;

	void SetTransitionScreenTransparent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * NovaRoyaleEvent;

	UFUNCTION(Client, WithValidation, Reliable)
	void PlayNovaRoyaleSound();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InstantiateEliminationWidget();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InstantiateStageChangeWidget();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InstantiateTimerScreen();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEliminationWidget> EliminationWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UStageChangeWidget> StageChangeWidgetClass;

	//Ranking for wwise...
	
	UPROPERTY(Replicated)
	FString PlayerName = "";

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Multicast_GetPlayerName();

	UFUNCTION(Client, WithValidation, Reliable)
	void MiniLeaderboard_GetPlayerName();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void MiniLeaderboard_FetchRanking(int ranking);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Server_GetTimer();

	UFUNCTION(Client, Reliable)
	void Client_GetTimer(float val);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Server_GetTimerState();

	UFUNCTION(Client, Reliable)
	void Client_GetTimerState(ETimerState val);

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_IncrementNumOfStageCompleteScreensAnimsPlayed();

	UFUNCTION(Client, Reliable)
	void Client_IncrementNumOfStageCompleteScreensAnimsPlayed();

	UEliminationWidget* EliminationWidget;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetEliminationWidgetNullptr();
};
