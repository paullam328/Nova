// Copywrite Shapeshifter 2019

#include "NetworkingGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "FindSessionsCallbackProxy.h"
#include "FighterBase.h"
#include "GameMapsSettings.h"
#include "FighterPlayerState.h"
#include "NovaGameSession.h"
#include "AkGameplayStatics.h"
#include "AkComponent.h"
#include "AkAudioEvent.h"

#include "Runtime/Engine/Classes/Engine/LocalPlayer.h"


UNetworkingGameInstance::UNetworkingGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/** Bind function for CREATING a Session */
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UNetworkingGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UNetworkingGameInstance::OnStartOnlineGameComplete);
	/** Bind function for DESTROYING a Session */
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UNetworkingGameInstance::OnDestroySessionComplete);
	/** Bind function for FINDING a Session */
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNetworkingGameInstance::OnFindSessionsComplete);
	/** Bind function for JOINING a Session */
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UNetworkingGameInstance::OnJoinSessionComplete);
	serverName = "NoName";

	

}

bool UNetworkingGameInstance::HostSession(FUniqueNetIdRepl UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers, bool bIsStageAdvancement)
{



	// Get the Online Subsystem to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
				Fill in all the Session Settings that we want to use.

				There are more with SessionSettings.Set(...);
				For example the Map or the GameMode/Type.
			*/
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, nMapName, EOnlineDataAdvertisementType::ViaOnlineService);
			SessionSettings->Set(SETTING_GAMEMODE, FString(TEXT("FightingGameMode")), EOnlineDataAdvertisementType::ViaOnlineService);

			if (!bIsStageAdvancement)
			{
				SessionSettings->Set(TEXT("CustomServerName"), serverName, EOnlineDataAdvertisementType::ViaOnlineService);
			}
			else
			{
				//change this later to allow multi-rooms
				FString stageStr = "Stage2";
				SessionSettings->Set(TEXT("CustomServerName"), stageStr, EOnlineDataAdvertisementType::ViaOnlineService);
			}

			//Test: Increase num of players:
			numOfPlayers++;
			SessionSettings->Set(TEXT("NumOfPlayers"), numOfPlayers, EOnlineDataAdvertisementType::ViaOnlineService);

			SessionSettings->Set(TEXT("IsStageAdvancement"), bIsStageAdvancement, EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void UNetworkingGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Sessions.IsValid() is %s"), (Sessions.IsValid() ? TEXT("True") : TEXT("False"))));

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("bWasSuccessful is %s"), (bWasSuccessful ? TEXT("True") : TEXT("False"))));
			if (bWasSuccessful)
			{

				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}

	}
}

void UNetworkingGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		//UNCOMMENT:
		UGameplayStatics::OpenLevel(GetWorld(), FName(*nMapName), true, "listen");
		//===========TEST===========================================================:

		//APlayerController * const PlayerController = GetFirstLocalPlayerController();

		//FString TravelURL;

		//if (PlayerController)
		//{
		//	PlayerController->ClientTravel(mapPath, ETravelType::TRAVEL_Absolute);
		//}
		//========================================================================
	}
}

void UNetworkingGameInstance::StartOnlineGame(FString mapName, bool bIsStageAdvancement)
{
	// Creating a local player where we can get the UserID from:
	ULocalPlayer* const Player = GetFirstGamePlayer();

	nMapName = mapName;

	// Call our custom HostSession function. GameSessionName is a GameInstance variable
	HostSession(Player->GetPreferredUniqueNetId(), /*GameSessionName*/"LevelVariableGameSession", true, true, 4, bIsStageAdvancement);
}

//Destroying session

void UNetworkingGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// If it was successful, we just load another level (could be a MainMenu!)
			if (bWasSuccessful)
			{
				//UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap", true);
			}
		}
	}
}

void UNetworkingGameInstance::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UNetworkingGameInstance::FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN, bool bIsPresence)
{
	// Get the OnlineSubsystem we want to work with
	//IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	UWorld* const World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

	if (World)
		//if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem: The below code is flawed as it returns null:
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		IOnlineSessionPtr Sessions = Online::GetSessionInterface(World);

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			/*
				Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch);

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			// We only want to set this Query Setting if "bIsPresence" is true
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("bIsPresence is: %d"), bIsPresence));
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			//OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void UNetworkingGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			//if (SessionSearch->SearchResults.Num() > 0)
			//{
			arrOfServerRowData.Empty();

			// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
			// This can be customized later on with your own classes to add more information that can be set and displayed
			for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
			{
				bool isStageAdvancement = false;
				SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(TEXT("IsStageAdvancement"), isStageAdvancement);

				if (!isStageAdvancement)
				{


					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!
					SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(TEXT("CustomServerName"), serverName);

					//Uncomment this if you wanna test the output of the session findings:
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
						FString::Printf(
							TEXT("Session Number: %d | Sessionname: %s | Customservername: %s | Number of Players now is: %d / %d"),
							SearchIdx + 1,
							*(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName),
							*serverName,
							SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections - SessionSearch->SearchResults[SearchIdx].Session.NumOpenPublicConnections,
							SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections
						));


					FServerData newdata;
					newdata.serverName = serverName;
					//newdata.numOfPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections - SessionSearch->SearchResults[SearchIdx].Session.NumOpenPublicConnections;
					newdata.numOfPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(TEXT("NumOfPlayers"), numOfPlayers);
					newdata.maxNumOfPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections;
					newdata.sessionName = SessionSearch->SearchResults[SearchIdx].Session.OwningUserName;
					arrOfServerRowData.Add(newdata);

					//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("arrOfServerData length is: %d")), arrOfServerRowData.Num());
				}
			}

			bStartUpdateServerRowData = true;
			areServersFound = true;


			//}
			//else
			//{
			//	areServersFound = false;
			//}
		}
	}
}

//Note: This is async:
void UNetworkingGameInstance::FindOnlineGames()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FindSessions(Player->GetPreferredUniqueNetId(), true, true);

	//TArray<FServerRowData> serverRowDatas = arrOfServerRowData;
	//arrOfServerRowData.Empty(); //Empty the array for future usage
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("This is async???")));
}

void UNetworkingGameInstance::SetServerName(FString name)
{
	serverName = name;
}

FString UNetworkingGameInstance::GetServerName()
{
	return serverName;
}

TArray<FServerData> UNetworkingGameInstance::UpdateArrOfServerRowData()
{
	TArray<FServerData> serverRowDatas;
	if (bStartUpdateServerRowData)
	{
		serverRowDatas = arrOfServerRowData;
		arrOfServerRowData.Empty();
	}
	return serverRowDatas;
}


bool UNetworkingGameInstance::JoinSessions(FUniqueNetIdRepl UserId, FName SessionName, const FOnlineSessionSearchResult & SearchResult)
{
	// Return bool
	bool bSuccessful = false;

	UWorld* const World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

	if (World)
		//if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem: The below code is flawed as it returns null:
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		IOnlineSessionPtr Sessions = Online::GetSessionInterface(World);

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}

		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Whether join is successful: %s"), (bSuccessful ? TEXT("true") : TEXT("false"))));

	}



	return bSuccessful;
}

void UNetworkingGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	// Get the OnlineSubsystem we want to work with
	UWorld* const World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

	if (World)
		//if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem: The below code is flawed as it returns null:
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		IOnlineSessionPtr Sessions = Online::GetSessionInterface(World);

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			//Test:
			if (Result == EOnJoinSessionCompleteResult::Success)
			{
				//GetDefault<UGameMapsSettings>()->TransitionMap.SetPath(mapPath);


				// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
				// This is something the Blueprint Node "Join Session" does automatically!
				APlayerController * const PlayerController = GetFirstLocalPlayerController();

				// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
				// String for us by giving him the SessionName and an empty String. We want to do this, because
				// Every OnlineSubsystem uses different TravelURLs
				FString TravelURL;

				FString tempStr = SessionName.ToString();
				UE_LOG(LogTemp, Warning, TEXT("NAME_GameSession is %s"), *tempStr);

				//			UGameMapsSettings::TransitionMap

							//if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
				if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
				{
					// Finally call the ClienTravel. If you want, you could print the TravelURL to see
					// how it really looks like

					//Uncomment this for join session map testing:
					//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Join session: traveling to %s")), *TravelURL);

					//PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Relative,true);

					//PlayerController->ClientTravel(Sessions->GetSessionSettings().Get("MAPNAME"), ETravelType::TRAVEL_Absolute);

					//For Testing purpose:
					//Test: Increase num of players:



					numOfPlayers = Sessions->GetSessionSettings(SessionName)->Get(TEXT("NumOfPlayers"), numOfPlayers);
					numOfPlayers++;

					UE_LOG_ONLINE_SESSION(Warning, TEXT("Join session: traveling to %s"), *TravelURL);
					Sessions->GetSessionSettings(SessionName)->Set(TEXT("NumOfPlayers"), numOfPlayers, EOnlineDataAdvertisementType::ViaOnlineService);
					//Uncomment below, as it may be the culprit:
					//PlayerController->ClientTravel(mapPath, ETravelType::TRAVEL_Absolute);
					//PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Relative, true);
					
					PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
					//Sessions->OnJoinSessionCompleteDelegates(Result);
				}
			}
		}
	}
}

void UNetworkingGameInstance::JoinOnlineGame(FString sessionName, bool bIsStageAdvancement)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	// Just a SearchResult where we can save the one we want to use, for the case we find more than one!
	FOnlineSessionSearchResult SearchResult;

	mapPath = "";
	// If the Array is not empty, we can go through it

	if (bIsStageAdvancement)
	{
		//at first, find dat session:


		/*
			Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
		*/
				SessionSearch = MakeShareable(new FOnlineSessionSearch);

				SessionSearch->bIsLanQuery = true;
				SessionSearch->MaxSearchResults = 20;
				SessionSearch->PingBucketSize = 50;

				// We only want to set this Query Setting if "bIsPresence" is true
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("bIsPresence is: %d"), bIsPresence));

				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

				TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

				// Set the Delegate to the Delegate Handle of the FindSession function
				//OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

				// Finally call the SessionInterface function. The Delegate gets called once this is finished
				Online::GetSessionInterface(GetWorld())->FindSessions(*Player->GetPreferredUniqueNetId(), SearchSettingsRef);


		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{

				bool isStageAdvancement = false;
				SessionSearch->SearchResults[i].Session.SessionSettings.Get(TEXT("IsStageAdvancement"), isStageAdvancement);
				if (isStageAdvancement)
				{
					//THEN, JOIN:
					SearchResult = SessionSearch->SearchResults[i];

					//SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(TEXT("CustomServerName"), serverName);
					SearchResult.Session.SessionSettings.Get(SETTING_MAPNAME, mapPath);

					// Once we found sounce a Session that is not ours, just join it. Instead of using a for loop, you could
					// use a widget where you click on and have a reference for the GameSession it represents which you can use
					// here
					JoinSessions(Player->GetPreferredUniqueNetId(), FName(*SessionSearch->SearchResults[i].Session.OwningUserName), SearchResult);

					//Uncomment this if you wanna test the output of the session findings:
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
						FString::Printf(
							TEXT("Session Number: %d | Sessionname: %s | Customservername: %s | Number of Players now is: %d / %d"),
							i + 1,
							*(SessionSearch->SearchResults[i].Session.OwningUserName),
							*serverName,
							SessionSearch->SearchResults[i].Session.SessionSettings.NumPublicConnections - SessionSearch->SearchResults[i].Session.NumOpenPublicConnections,
							SessionSearch->SearchResults[i].Session.SessionSettings.NumPublicConnections
						));

					break;
				}
			}
		
	}

	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			// To avoid something crazy, we filter sessions from ourself

			if (!bIsStageAdvancement)
			{
				if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
				{
					SearchResult = SessionSearch->SearchResults[i];

					//SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(TEXT("CustomServerName"), serverName);
					SearchResult.Session.SessionSettings.Get(SETTING_MAPNAME, mapPath);

					// Once we found sounce a Session that is not ours, just join it. Instead of using a for loop, you could
					// use a widget where you click on and have a reference for the GameSession it represents which you can use
					// here
					JoinSessions(Player->GetPreferredUniqueNetId(), FName(*sessionName), SearchResult);
					break;
				}
			}
		}
	}

	// This can be customized later on with your own classes to add more information that can be set and displayed
	/*for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
	{
		bool isStageAdvancement = false;
		SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(TEXT("IsStageAdvancement"), isStageAdvancement);

		if (!isStageAdvancement)
		{


			// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
			// This is something you can't do in Blueprint for example!
			SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(TEXT("CustomServerName"), serverName);

			//Uncomment this if you wanna test the output of the session findings:
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
				FString::Printf(
					TEXT("Session Number: %d | Sessionname: %s | Customservername: %s | Number of Players now is: %d / %d"),
					SearchIdx + 1,
					*(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName),
					*serverName,
					SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections - SessionSearch->SearchResults[SearchIdx].Session.NumOpenPublicConnections,
					SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections
				));


			FServerData newdata;
			newdata.serverName = serverName;
			//newdata.numOfPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections - SessionSearch->SearchResults[SearchIdx].Session.NumOpenPublicConnections;
			newdata.numOfPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(TEXT("NumOfPlayers"), numOfPlayers);
			newdata.maxNumOfPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections;
			newdata.sessionName = SessionSearch->SearchResults[SearchIdx].Session.OwningUserName;
			arrOfServerRowData.Add(newdata);

			//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("arrOfServerData length is: %d")), arrOfServerRowData.Num());
		}
	}*/
}

void UNetworkingGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UNetworkingGameInstance, numOfPlayers);
	//DOREPLIFETIME(UNetworkingGameInstance, isWinLoseConditionDecided);
	DOREPLIFETIME(UNetworkingGameInstance, MapTransitionData);

	DOREPLIFETIME(UNetworkingGameInstance, WwiseTransitionData);

	DOREPLIFETIME(UNetworkingGameInstance, LeaderboardDataStack);

	DOREPLIFETIME(UNetworkingGameInstance, AkComp);
}

void UNetworkingGameInstance::SetPlayerData(int id)
{
	/*FPlayerData playerData;
	playerData.playerId = id;
	playerData.playerPawn = NULL;

	TArray<FPlayerData> arrayOfPlayerData;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions->GetSessionSettings(FName(*arrOfServerRowData[0].serverName))->Get("ArrayOfPlayerData", arrayOfPlayerData) == NULL)
		{
			arrayOfPlayerData.Add(playerData);
			Sessions->GetSessionSettings(FName(*arrOfServerRowData[0].serverName))->Set("ArrayOfPlayerData", arrayOfPlayerData, EOnlineDataAdvertisementType::ViaOnlineService);
		}
		else
		{
			Sessions->GetSessionSettings(FName(*arrOfServerRowData[0].serverName))->Get("ArrayOfPlayerData", arrayOfPlayerData);
			arrayOfPlayerData.Add(playerData);
			Sessions->GetSessionSettings(FName(*arrOfServerRowData[0].serverName))->Set("ArrayOfPlayerData", arrayOfPlayerData, EOnlineDataAdvertisementType::ViaOnlineService);
		}
	}*/
}

TArray<FPlayerData> UNetworkingGameInstance::GetArrayOfPlayerData()
{
	TArray<FPlayerData> arrayOfPlayerData;
	/*
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions->GetSessionSettings(FName(*arrOfServerRowData[0].serverName))->Get("ArrayOfPlayerData", arrayOfPlayerData) != NULL)
			Sessions->GetSessionSettings(FName(*arrOfServerRowData[0].serverName))->Get("ArrayOfPlayerData", arrayOfPlayerData);
	}*/
	return arrayOfPlayerData;
}

bool UNetworkingGameInstance::StoreMapTransitionData_Validate(const FString& PlayerName, int PlayerIndex, int SelectedCharIndex)
{
	return true;
}


void UNetworkingGameInstance::StoreMapTransitionData_Implementation(const FString& PlayerName, int PlayerIndex, int SelectedCharIndex)
{
	FMapTransitionData data;
	data.SetPlayerName(PlayerName);
	data.SetPlayerIndex(PlayerIndex);
	data.SetSelectedCharIndex(SelectedCharIndex);

	//Set it!
	MapTransitionData.Add(data);
}


bool UNetworkingGameInstance::StoreLeaderboardData_Validate(const FString& PlayerName, int PlayerIndex, int SelectedCharIndex, int Aggression)
{
	return true;
}

void UNetworkingGameInstance::StoreLeaderboardData_Implementation(const FString& PlayerName, int PlayerIndex, int SelectedCharIndex, int Aggression)
{

		FLeaderboardData data;
		data.SetPlayerName(PlayerName);
		data.SetPlayerIndex(PlayerIndex);
		data.SetSelectedCharIndex(SelectedCharIndex);
		data.SetAggression(Aggression);

		//Set it!
		LeaderboardDataStack.Add(data);

}

void UNetworkingGameInstance::StoreWwiseTransitionData(float mSliderVal, float sSliderVal, float vSliderVal)
{
	MusSliderVal = mSliderVal;
	SFXSliderVal = sSliderVal;
	VoiceSliderVal = vSliderVal;

}

bool UNetworkingGameInstance::SetAkComp_Validate(UAkComponent* comp)
{
	return true;
}


void UNetworkingGameInstance::SetAkComp_Implementation(UAkComponent* comp)
{
	if (comp != nullptr)
	{
		AkComp = comp;
	}
}

void UNetworkingGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UNetworkingGameInstance::PlayWwiseTransitionVoice);
}


void UNetworkingGameInstance::PlayWwiseTransitionVoice(const FString& MapName)
{
	Multicast_PlayWwiseTransitionVoice();
}

bool UNetworkingGameInstance::Multicast_PlayWwiseTransitionVoice_Validate()
{
	return true;
}

void UNetworkingGameInstance::Multicast_PlayWwiseTransitionVoice_Implementation()
{
	//UAkComponent * AkComp = UAkGameplayStatics::SpawnAkComponentAtLocation(GetWorld(), TransitionEvent, nullptr, FVector(0, 0, 0), FRotator(0, 0, 0), true, "", "", false);
}

void UNetworkingGameInstance::DisconnectFromServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Disconnecting and returning to MainMenu"));
	ReturnToMainMenu();
}


void UNetworkingGameInstance::RegisterSteamMasterServer()
{
	////ANovaGameSession* GameSteamSession = Cast<AGameSession>();

	//// Checking the NetMode to ensure that this is only executed in the Dedicated Server

	//if (GetWorld()->GetNetMode() != NM_DedicatedServer)
	//{
	//	return; 
	//}

	//// Getting the FUniqueNetID...
	//TSharedPtr<const FUniqueNetId> UserID;

	//// Gettins the Session interface
	//IOnlineSessionPtr Sessions = Online::GetSessionInterface();

	//if (Sessions.IsValid())
	//{
	//	// Set the Delegate to the Delegate Handle of the FindSession function
	//	OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	//	/*
	//		Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
	//	*/
	//	SessionSearch = MakeShareable(new FOnlineSessionSearch);

	//	SessionSearch->MaxSearchResults = 20;
	//	SessionSearch->PingBucketSize = 50;

	//	// We only want to set this Query Setting if "bIsPresence" is true
	//	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("bIsPresence is: %d"), bIsPresence))

	//	TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

	//	// Set the Delegate to the Delegate Handle of the FindSession function
	//	//OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	//	// Finally call the SessionInterface function. The Delegate gets called once this is finished
	//	Sessions->FindSessions(*UserID, SearchSettingsRef);


	//	// We Try to see the session Results 
	//	if (SessionSearch->SearchResults.Num() > 0)
	//	{
	//		for (int32 SearchIndex = 0; SearchIndex < SessionSearch->SearchResults.Num(); SearchIndex++)
	//		{
	//		
	//			ANovaGameSession* OurSession = Cast<ANovaGameSession>(SessionSearch->SearchResults[SearchIndex]);



	//		}
	//	}


	//}

}