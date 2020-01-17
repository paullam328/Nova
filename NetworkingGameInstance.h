// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UObject/CoreOnline.h"
#include "EngineGlobals.h"
#include "UnrealNetwork.h"
#include "OnlineSessionInterface.h"
#include "GameFramework/OnlineReplStructs.h"
#include "MapTransitionData.h"
#include "LeaderboardData.h"
#include "WwiseTransitionData.h"

#include "NetworkingGameInstance.generated.h"


class AFighterBase;
class OnlineSubsystem;
class OnlineSubsystemUtils;
class OnlineSessionSettings;

class AFighterPlayerState;
class UAkComponent;
class UAkGameplayStatics;
class UAkAudioEvent;

/**
 * 
 */


/*
	This will contain data per Server Room
*/

USTRUCT(BlueprintType)
struct FServerData
{

	GENERATED_BODY()

	//CUSTOMIZED Server Name:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Row Data")
	FString serverName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Row Data")
	int32 numOfPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Row Data")
	int32 maxNumOfPlayers;

	//Need this to join server:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Row Data")
	FString sessionName;

	FServerData()
	{
		serverName = "";
		numOfPlayers = 0;
		maxNumOfPlayers = 6;
	}
};


/*
	This will contain the network information of Players
*/

USTRUCT(BlueprintType)
struct FPlayerData
{

	// This struct data is experimental, it would be removed and replaced with a 
	// Reference with the PlayerState class which withold all the information per character


	GENERATED_BODY()


	// Player ID of each player that joins, this could be computed hash...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	int32 PlayerID;

	
	// Putting them under a class of FighterBase..
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	TSubclassOf<class AFighterBase> playerPawn;


	FPlayerData()
	{
		PlayerID = -1;
		playerPawn = NULL;
	}

};




UCLASS()
class NOVAFINAL_API UNetworkingGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:


	UNetworkingGameInstance(const FObjectInitializer& ObjectInitializer);

	int32 characterNum = 10;
	//============================================================================================
	//Hosting Session:
		/**
	*	Function to host a game!
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/

	bool HostSession(FUniqueNetIdRepl UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers, bool bIsStageAdvancement);

	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/**
*	Function fired when a session create request has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	//Settings that our Session will have (like LAN or Number of allowed Players):
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	//Creating a session:
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void StartOnlineGame(FString mapName, bool bIsStageAdvancement);

	FString nMapName;

	//============================================================================================
	//Destroying Session:

	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/**
*	Delegate fired when a destroying an online session has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Network")
		void DestroySessionAndLeaveGame();

	//============================================================================================
	//Searching and finding a session:

	/**
	*	Find an online session
	*
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*	@param bIsPresence are we searching presence sessions
	*/
	void FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN, bool bIsPresence);

	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	//Contain our SearchResults, once this search is complete and successful
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Network")
		//void FindOnlineGames();
		void FindOnlineGames();

	FString serverName;

	bool areServersFound = false;

	UFUNCTION(BlueprintCallable, Category = "Network")
		void SetServerName(FString name);

	UFUNCTION(BlueprintCallable, Category = "Network")
		FString GetServerName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ServerRowData")
		TArray<FServerData> arrOfServerRowData;

	UFUNCTION(BlueprintCallable, Category = "Network")
		TArray<FServerData> UpdateArrOfServerRowData();
	//Set this to false after updating the data, to reset the data-to-set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ServerRowData")
		bool bStartUpdateServerRowData = false;

	//============================================================================================

	//Joining a session:

	/**
	*	Joins a session via a search result
	*
	*	@param SessionName name of session
	*	@param SearchResult Session to join
	*
	*	@return bool true if successful, false otherwise.
	*/

	//bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) override;

	bool JoinSessions(FUniqueNetIdRepl UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/**
*	Delegate fired when a session join request has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void JoinOnlineGame(FString sessionName, bool bIsStageAdvancement);

	FString mapPath = "";
	//============================================================================================

	//For Testing Purposes:

	UPROPERTY(Replicated)
		int numOfPlayers = 0;

	UFUNCTION(BlueprintCallable, Category = "Player Data")
		void SetPlayerData(int id);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
		TArray<FPlayerData> GetArrayOfPlayerData();

	//UPROPERTY(ReplicatedUsing)
		bool isWinLoseConditionDecided = false;


//For player data in map transitions
public:

	UFUNCTION()
	void RegisterSteamMasterServer();


	UPROPERTY(Replicated)
	TArray<FMapTransitionData> MapTransitionData;

	UFUNCTION(Server, WithValidation, Reliable)
	void StoreMapTransitionData(const FString& PlayerName, int PlayerIndex, int SelectedCharIndex);
	
	//Losers in, winner out: LIFO!
	UPROPERTY(Replicated)
	TArray<FLeaderboardData> LeaderboardDataStack;

	UFUNCTION(Server, WithValidation, Reliable)
	void StoreLeaderboardData(const FString& PlayerName, int PlayerIndex, int SelectedCharIndex, int Aggression);


	UFUNCTION(BlueprintCallable, Category = "Networking")
	void DisconnectFromServer();


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<FWwiseTransitionData> WwiseTransitionData;

	//UFUNCTION(Server, WithValidation, Reliable)
	//void StoreWwiseTransitionData(const FString& PlayerName, float MusSliderVal, float SFXSliderVal, float VoiceSliderVal);
	void StoreWwiseTransitionData(float mSliderVal, float sSliderVal, float vSliderVal);

	float MusSliderVal;
	float SFXSliderVal;
	float VoiceSliderVal;

	UPROPERTY(Replicated)
	UAkComponent* AkComp;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetAkComp(UAkComponent* comp);


	//Loading Screen:
	virtual void Init() override;

	UFUNCTION()
	virtual void PlayWwiseTransitionVoice(const FString& MapName);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Multicast_PlayWwiseTransitionVoice();

	UPROPERTY(EditAnywhere, Category = "Wwise")
	UAkAudioEvent* TransitionEvent;
};
