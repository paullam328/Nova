// Fill out your copyright notice in the Description page of Project Settings.

#include "FightingGameMode.h"
#include "Engine.h"
#include "FighterBase.h"
#include "NetworkingGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/NetDriver.h"
#include "MapTransitionData.h"
#include "FighterPlayerState.h"
#include "DemarkBase.h"
#include "MaskiumBase.h"
#include "FightingGameState.h"
#include "FighterController.h"

#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"


AFightingGameMode::AFightingGameMode()
{
	bActorSeamlessTraveled = true;

	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		UNetDriver* Driver = World->GetNetDriver();


		if (Driver != nullptr)
		{
			Driver->bClampListenServerTickRate = true;
		}
	}

	bUseSeamlessTravel = false;
}



void AFightingGameMode::SetCharNum(int32 val)
{
	charNum = val;
}

bool AFightingGameMode::ReadyToStartMatch_Implementation()
{
	//Super::ReadyToStartMatch();

	return MaxNumPlayers == NumPlayers;
}

FString AFightingGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	AFighterPlayerState* PS = Cast<AFighterPlayerState>(NewPlayerController->PlayerState);

	// Look for auto-join options.  Don't attempt to auto-join until we receive all of the relevant information from the client though
	if (PS)
	{
		//FString InstanceID = UGameplayStatics::ParseOption(Options, "Session");
		//if (!InstanceID.IsEmpty())
		//{
		//	// Can't use the playerstate's version here because it hasn't been set yet.
		//	PS->bDesiredJoinAsSpectator = FCString::Stricmp(*UGameplayStatics::ParseOption(Options, TEXT("SpectatorOnly")), TEXT("1")) == 0;
		//	PS->DesiredMatchIdToJoin = InstanceID;
		//}

		//FString FriendId = UGameplayStatics::ParseOption(Options, TEXT("Friend"));
		//if (!FriendId.IsEmpty())
		//{
		//	PS->DesiredFriendToJoin = FriendId;
		//}

		//FString InOpt = UGameplayStatics::ParseOption(Options, TEXT("Character"));
		//if (InOpt.Len() > 0)
		//{
		//	PS->SetCharacter(InOpt);
		//}

		//PS->PartyLeader = UGameplayStatics::ParseOption(Options, TEXT("PartyLeader"));
		//PS->PartySize = UGameplayStatics::GetIntOption(Options, TEXT("PartySize"), 1);
	}

	return Result;
}

void AFightingGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//FString LocalServerAddress = FString(TEXT("127.0.0.1"));
	//if (Address.Compare(LocalServerAddress) && IsRunningDedicatedServer())
	//{
	//	//ErrorMessage = "This is the Dedicated Server!, Kicking out...";
	//}
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* AFightingGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	return PC;
}
void AFightingGameMode::HandleSeamlessTravelPlayer(AController *& C)
{
	Super::HandleSeamlessTravelPlayer(C);
	
	APlayerController* NewPlayer = Cast<APlayerController>(C);

	PlayerControllerList.Add(NewPlayer);

	UWorld* World = GetWorld();


	FActorSpawnParameters SpawnParams;
	FRotator rotator;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	if (World)
	{
		/*ENetMode Mode = World->GetNetMode();
		if (Mode == NM_DedicatedServer)
		{
			return;
		}
		else if(Role == ROLE_AutonomousProxy || (Role == ROLE_Authority && GetRemoteRole() < ROLE_AutonomousProxy))
		{*/
		int numOfPlayers = PlayerControllerList.Num() - 1;

		FString Position = FString::FromInt(numOfPlayers);

		TSubclassOf<class AFighterBase> characterToSpawn;

		UNetworkingGameInstance* GI = Cast<UNetworkingGameInstance>(GetWorld()->GetGameInstance());
		if (GI != nullptr)
		{
			AFighterPlayerState* FPS = NewPlayer->GetPlayerState<AFighterPlayerState>();
			//If == 0, then it must be dev mode...
			if (GI->MapTransitionData.Num() > 0)
			{
				for (int i = 0; i < GI->MapTransitionData.Num(); i++)
				{
					if (FPS->GetPlayerName() == GI->MapTransitionData[i].PlayerName)
					{
						switch (GI->MapTransitionData[i].SelectedCharIndex)
						{
						case 0:
							characterToSpawn = Demark;
							break;
						case 1:
							characterToSpawn = Maskium;
							break;
						default:
							characterToSpawn = Demark;
							break;
						}
						FPS->allCharacterIndexValues.Add(GI->MapTransitionData[i].SelectedCharIndex);
						FPS->PlayerIndex = GI->MapTransitionData[i].PlayerIndex;
					}
				}

				APawn* Player = World->SpawnActor<AFighterBase>(characterToSpawn, FindPlayerStart(NewPlayer, Position)->GetActorTransform(), SpawnParams);
				//For the color:
				if (Cast<ADemarkBase>(Player) != nullptr)
				{
					ADemarkBase* DB = Cast<ADemarkBase>(Player);
					if (DB != nullptr)
					{
						DB->PlayerNum = FPS->PlayerIndex;
						DB->SERVER_SetCharColor(FPS->PlayerIndex);
					}
				}
				else if (Cast<AMaskiumBase>(Player) != nullptr)
				{
					AMaskiumBase* MB = Cast<AMaskiumBase>(Player);
					if (MB != nullptr)
					{
						MB->PlayerNum = FPS->PlayerIndex;
						MB->SERVER_SetCharColor(FPS->PlayerIndex);
					}
				}

				NewPlayer->Possess(Player);

				//Implement these in gamestate instead:

				//RetrieveWwiseData(GI);
				SetNumOfSelectionScreenPlayers(GI->MapTransitionData.Num());
				IncrementIsCharSetCounterAndCheckIfAllReady();
			}
			else
			{
				ENetMode Mode = World->GetNetMode();

				if (Mode == NM_DedicatedServer)
				{
					return;
				}

				//Dev Mode: ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				characterToSpawn = Demark;
				AFightingGameState* FGS = GetGameState<AFightingGameState>();
				if (FGS != nullptr)
				{
					switch (FGS->charToTest)
					{
					case 0:
						characterToSpawn = Demark;
						break;
					case 1:
						characterToSpawn = Maskium;
						break;
					default:
						characterToSpawn = Demark;
						break;
					}
				}
				FPS->PlayerIndex = FCString::Atoi(*Position);

				APawn* Player = World->SpawnActor<AFighterBase>(characterToSpawn, FindPlayerStart(NewPlayer, Position)->GetActorTransform(), SpawnParams);
				//For the color:
				//ADemarkBase* DB = Cast<ADemarkBase>(Player);
				//if (DB != nullptr)
				//{
				//	DB->PlayerNum = FPS->PlayerIndex;
				//	DB->SERVER_SetCharColor(FPS->PlayerIndex);

				//	//Set to pre-set numOfSelectionScreenPlayers
				//	SetNumOfSelectionScreenPlayers(Cast<AFightingGameState>(UGameplayStatics::GetGameState(GetWorld()))->numOfSelectionScreenPlayers);
				//	IncrementIsCharSetCounterAndCheckIfAllReady();
				if (Cast<ADemarkBase>(Player) != nullptr)
				{
					ADemarkBase* DB = Cast<ADemarkBase>(Player);
					if (DB != nullptr)
					{
						DB->PlayerNum = FPS->PlayerIndex;
						DB->SERVER_SetCharColor(FPS->PlayerIndex);
					}
				}
				else if (Cast<AMaskiumBase>(Player) != nullptr)
				{
					AMaskiumBase* MB = Cast<AMaskiumBase>(Player);
					if (MB != nullptr)
					{
						MB->PlayerNum = FPS->PlayerIndex;
						MB->SERVER_SetCharColor(FPS->PlayerIndex);
					}
				}

				NewPlayer->Possess(Player);
			}
			/*}*/

			// With the Menu Selection the UI Should respond the blueprint of the character to spawn


			/*if (FPlatformProperties::IsClientOnly())
			{

			}*/
			//FPS->Trigger_InstantiateStageSwitchWipeWidget();

		}

	}
}

/*
void AFightingGameMode::HandleStartingNewPlayer(APlayerController * NewPlayer)
{
	Super::HandleStartingNewPlayer(NewPlayer);
}
*/
void AFightingGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//Super::HandleSeamlessTravelPlayer(C);

	//Player transition screen:
	//Below never gets called in client... probably requires seamless travel
	//Cast<AFighterController>(NewPlayer)->Server_InstantiateStageSwitchWipeWidget(false);

	//APlayerController* ANewPlayer = Cast<APlayerController>(NewPlayer);
	
	PlayerControllerList.Add(NewPlayer);

	UWorld* World = GetWorld();


	FActorSpawnParameters SpawnParams;
	FRotator rotator;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	if (World)
	{
		/*ENetMode Mode = World->GetNetMode();
		if (Mode == NM_DedicatedServer)
		{
			return;
		}
		else if(Role == ROLE_AutonomousProxy || (Role == ROLE_Authority && GetRemoteRole() < ROLE_AutonomousProxy))
		{*/
			int numOfPlayers = PlayerControllerList.Num() - 1;

			FString Position = FString::FromInt(numOfPlayers);

			TSubclassOf<class AFighterBase> characterToSpawn;

			UNetworkingGameInstance* GI = Cast<UNetworkingGameInstance>(GetWorld()->GetGameInstance());
			if (GI != nullptr)
			{
				AFighterPlayerState* FPS = NewPlayer->GetPlayerState<AFighterPlayerState>();
				//If == 0, then it must be dev mode...
				if (GI->MapTransitionData.Num() > 0)
				{
					for (int i = 0; i < GI->MapTransitionData.Num(); i++)
					{
						if (FPS->GetPlayerName() == GI->MapTransitionData[i].PlayerName)
						{
							switch (GI->MapTransitionData[i].SelectedCharIndex)
							{
							case 0:
								characterToSpawn = Demark;
								break;
							case 1:
								characterToSpawn = Maskium;
								break;
							default:
								characterToSpawn = Demark;
								break;
							}
							FPS->PlayerIndex = GI->MapTransitionData[i].PlayerIndex;
						}
					}

					APawn* Player = World->SpawnActor<AFighterBase>(characterToSpawn, FindPlayerStart(NewPlayer, Position)->GetActorTransform(), SpawnParams);
					//For the color:
					if (Cast<ADemarkBase>(Player) != nullptr)
					{
						ADemarkBase* DB = Cast<ADemarkBase>(Player);
						if (DB != nullptr)
						{
							DB->PlayerNum = FPS->PlayerIndex;
							DB->SERVER_SetCharColor(FPS->PlayerIndex);
						}
					}
					else if (Cast<AMaskiumBase>(Player) != nullptr)
					{
						AMaskiumBase* MB = Cast<AMaskiumBase>(Player);
						if (MB != nullptr)
						{
							MB->PlayerNum = FPS->PlayerIndex;
							MB->SERVER_SetCharColor(FPS->PlayerIndex); 
						}
					}

					NewPlayer->Possess(Player);

					//Implement these in gamestate instead:
					SetNumOfSelectionScreenPlayers(GI->MapTransitionData.Num());
					IncrementIsCharSetCounterAndCheckIfAllReady();
				}
				else
				{
					ENetMode Mode = World->GetNetMode();

					if (Mode == NM_DedicatedServer)
					{
						return;
					}

					//Dev Mode: ++++++++++++++++++++++++++++++++++++++++++++++++++++++++

					AFightingGameState* FGS = GetGameState<AFightingGameState>();
					switch (FGS->charToTest)
					{
						case 0:
							characterToSpawn = Demark;
							break;
						case 1:
							characterToSpawn = Maskium;
							break;
						default:
							characterToSpawn = Demark;
							break;
					}
					FPS->PlayerIndex = FCString::Atoi(*Position);

					APawn* Player = World->SpawnActor<AFighterBase>(characterToSpawn, FindPlayerStart(NewPlayer, Position)->GetActorTransform(), SpawnParams);
					
					//For the color:
					if (Cast<ADemarkBase>(Player) != nullptr)
					{
						ADemarkBase* DB = Cast<ADemarkBase>(Player);
						if (DB != nullptr)
						{
							DB->PlayerNum = FPS->PlayerIndex;
							DB->SERVER_SetCharColor(FPS->PlayerIndex);
						}
					}
					else if (Cast<AMaskiumBase>(Player) != nullptr)
					{
						AMaskiumBase* MB = Cast<AMaskiumBase>(Player);
						if (MB != nullptr)
						{
							MB->PlayerNum = FPS->PlayerIndex;
							//MB->SERVER_SetCharColor(FPS->PlayerIndex); this line breaks
						}
					}
					//Set to pre-set numOfSelectionScreenPlayers
					SetNumOfSelectionScreenPlayers(Cast<AFightingGameState>(UGameplayStatics::GetGameState(GetWorld()))->numOfSelectionScreenPlayers);
					IncrementIsCharSetCounterAndCheckIfAllReady();
					NewPlayer->Possess(Player);
				}
			/*}*/

			// With the Menu Selection the UI Should respond the blueprint of the character to spawn

				
			/*if (FPlatformProperties::IsClientOnly())
			{

			}*/
				//FPS->Trigger_InstantiateStageSwitchWipeWidget();

			}

	}
}

void AFightingGameMode::BeginPlay()
{
	Super::BeginPlay();


	//  This is used to convert the options of how many max players that are going to be in the game 
	//  Will take an 'FString' and convert it to 'int32'
	// 'UGameplayStatiscs' Class to get the correct Key from the 'OptionString'

	MaxNumPlayers = FCString::Atoi(*(UGameplayStatics::ParseOption(OptionsString, "MaxNumPlayers")));
}

// Handeling the Leaving out the GameMode
void AFightingGameMode::Logout(AController* Exiting)
{
	APlayerController* Controller = Cast<APlayerController>(Exiting);

	if (Controller != nullptr)
	{
		PlayerControllerList.Remove(Controller);

	}
}

bool AFightingGameMode::RetrieveWwiseData_Validate(UNetworkingGameInstance* GI)
{
	return true;
}

void AFightingGameMode::RetrieveWwiseData_Implementation(UNetworkingGameInstance* GI)
{
	if (Role == ROLE_Authority)
	{
		for (int i = 0; i < GI->WwiseTransitionData.Num(); i++)
		{
			for (int j = 0; j < PlayerControllerList.Num(); j++)
			{
				AFighterController* FC = Cast<AFighterController>(PlayerControllerList[j]);
				if (GI->WwiseTransitionData[i].GetPlayerName() == FC->GetPlayerState<AFighterPlayerState>()->GetPlayerName())
				{
					//FC->MusSliderVal = GI->WwiseTransitionData[i].GetMusSliderVal();
					//FC->SFXSliderVal = GI->WwiseTransitionData[i].GetSFXSliderVal();
					//FC->VoiceSliderVal = GI->WwiseTransitionData[i].GetVoiceSliderVal();
				}
			}
		}
	}
}


//void AFightingGameMode::RunTimer(float DeltaTime)
//{
	//InternalTimer += DeltaTime;

	//// Checking if its 5 seconds past and resetting the GameMode
	//if (InternalTimer > 5.f)
	//{
	//	ResetIfNoPlayers();
	//}
	//else
	//{
	//	return;
	//}
//}

//void AFightingGameMode::ResetIfNoPlayers()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Running ResetIfNoPlayers"));
//	// Checking if the Map has no players, we move back to character selction after some time
//	if (GetNetMode() == NM_DedicatedServer)
//	{
//		if (PlayerControllerList.Num() == 0)
//		{
//			UWorld* World = GetWorld();
//
//			if (World != nullptr)
//			{
//				UE_LOG(LogTemp, Warning, TEXT("Attempting ResetIfNoPlayers"));
//				World->ServerTravel("/Game/Maps/CharacterSelection?listen", true, false);
//			}
//		}
//	}
//	else
//	{
//		return;
//	}
//}

void AFightingGameMode::Tick(float DeltaTime)
{
	//RunTimer(DeltaTime);
}

TArray<class APlayerController*> AFightingGameMode::PlayerArray()
{
	return PlayerControllerList;
}

void AFightingGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFightingGameMode, isCharSetCounter);
	DOREPLIFETIME(AFightingGameMode, numOfSelectionScreenPlayers);
	DOREPLIFETIME(AFightingGameMode, isEveryPlayerLoaded);
}

bool AFightingGameMode::IncrementIsCharSetCounterAndCheckIfAllReady_Validate()
{
	return true;
}

void AFightingGameMode::IncrementIsCharSetCounterAndCheckIfAllReady_Implementation()
{
	if (Role == ROLE_Authority)
	{
		isCharSetCounter += 1;
		if (isCharSetCounter == numOfSelectionScreenPlayers)
		{
			isEveryPlayerLoaded = true;
		}
	}
}

bool AFightingGameMode::SetNumOfSelectionScreenPlayers_Validate(int num)
{
	return true;
}

void AFightingGameMode::SetNumOfSelectionScreenPlayers_Implementation(int num)
{
	if (Role == ROLE_Authority)
	{
		numOfSelectionScreenPlayers = num;
	}
}


bool AFightingGameMode::Trigger_ServerTravel_Validate()
{
	return true;
}

void AFightingGameMode::Trigger_ServerTravel_Implementation()
{
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = false;
	World->ServerTravel("/Game/Maps/LeaderboardMap?listen", true, false);
}