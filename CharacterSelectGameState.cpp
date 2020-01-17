// Copywrite Shapeshifter 2019

#include "CharacterSelectGameState.h"
#include "CharacterSelectWidget.h"
#include "NetworkingGameInstance.h"
#include "CharacterSelectionGameMode.h"
#include "CharacterSelectController.h"
#include "GameFramework/OnlineReplStructs.h"
#include "FighterPlayerState.h"
#include "Engine.h"
#include "AkAudioEvent.h"
#include "AkGameplayStatics.h"


ACharacterSelectGameState::ACharacterSelectGameState()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}

void ACharacterSelectGameState::BeginPlay()
{
	//SetPlayerIndex(); //Cannot do this here since PlayerArray is empty (prob inaccessible)...
	
	InitializeCharIndices();
	InitializePlayersReady();
	InitializeTimerState(ECharacterSelectTimerState::WaitingForTwoPlayers);
}

void ACharacterSelectGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetPlayerIndex();
	RunTimer(DeltaTime);
	CallCountdown(DeltaTime);
}

bool ACharacterSelectGameState::SetPlayerIndex_Validate()
{
	return true;
}

void ACharacterSelectGameState::SetPlayerIndex_Implementation()
{
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();

		if (World != nullptr)
		{

			for (int i = 0; i < PlayerArray.Num(); i++)
			{
				//UPlayer* Player = PlayerArray[i]->GetNetOwningPlayer();
				//APlayerController* Controller = Player->GetPlayerController(World);
				//ACharacterSelectController* Controller = Cast<ACharacterSelectController>(Player->GetPlayerController(World));
				//ACharacterSelectController* Controller = Cast<ACharacterSelectController>(UGameplayStatics::GetPlayerController(World, 0));
				ACharacterPlayerState* CPS = Cast<ACharacterPlayerState>(PlayerArray[i]);
				if (CPS != nullptr)
				{
					CPS->SetPlayerIndex(i);
				}
			//	if (Controller != nullptr)
				//{
					//ACharacterSelectController* CSP = Cast<ACharacterSelectController>(Controller);
					//if (!CSP->IsIndexSet)
					//{
					//	CSP->PlayerIndex = i;
					//	CSP->IsIndexSet = true;
					//}
				//}
				else
				{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Controller is nullptr")));

				}
			}
		}
	}
}

bool ACharacterSelectGameState::SetSelectedCharPerPlayer_Validate(int PlayerIndex, ECharacter character)
{
	return true;
}

//Call this on left/right button trigger in the CharacterSelectPawn function
void ACharacterSelectGameState::SetSelectedCharPerPlayer_Implementation(int PlayerIndex, ECharacter character)
{
	if (Role == ROLE_Authority)
	{

		if (ArePlayersReady.IsValidIndex(PlayerIndex) && SelectedCharIndices.IsValidIndex(PlayerIndex))
		{
			if (ArePlayersReady[PlayerIndex] == EPlayerReady::NotReady)
			{
				switch (character)
				{
				case ECharacter::Demark:
					SelectedCharIndices[PlayerIndex] = 0;
					break;

				case ECharacter::Maskium:
					SelectedCharIndices[PlayerIndex] = 1;
					break;

				default:
					SelectedCharIndices[PlayerIndex] = 0; //spawns demark for now until we get other characters
					break;
				}
			}
		}
		
	}
}

bool ACharacterSelectGameState::InitializeCharIndices_Validate()
{
	return true;
}

void ACharacterSelectGameState::InitializeCharIndices_Implementation()
{
	if (Role == ROLE_Authority)
	{
		SelectedCharIndices.Add(-1);
		SelectedCharIndices.Add(-1);
		SelectedCharIndices.Add(-1);
		SelectedCharIndices.Add(-1);
		SelectedCharIndices.Add(-1);
		SelectedCharIndices.Add(-1);
	}
}

bool ACharacterSelectGameState::InitializePlayersReady_Validate()
{
	return true;
}

void ACharacterSelectGameState::InitializePlayersReady_Implementation()
{
	if (Role == ROLE_Authority)
	{
		ArePlayersReady.Add(EPlayerReady::NonExistent);
		ArePlayersReady.Add(EPlayerReady::NonExistent);
		ArePlayersReady.Add(EPlayerReady::NonExistent);
		ArePlayersReady.Add(EPlayerReady::NonExistent);
		ArePlayersReady.Add(EPlayerReady::NonExistent);
		ArePlayersReady.Add(EPlayerReady::NonExistent);
	}
}

bool ACharacterSelectGameState::SetPlayerReadyPerPlayer_Validate(int PlayerIndex)
{
	return true;
}

//Call this on left/right button trigger in the CharacterSelectPawn function
void ACharacterSelectGameState::SetPlayerReadyPerPlayer_Implementation(int PlayerIndex)
{
	if (Role == ROLE_Authority)
	{

		if (ArePlayersReady.IsValidIndex(PlayerIndex))
		{
			ArePlayersReady[PlayerIndex] = EPlayerReady::Ready;

			int readyCounter = 0;
			for (int i = 0; i < ArePlayersReady.Num(); i++)
			{
				if (ArePlayersReady[i] == EPlayerReady::Ready) readyCounter++;
			}
			if (readyCounter == PlayerArray.Num())
			{
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("ALL PLAYERS ARE READY!"));

					//Store transition data:
					UWorld* World = GetWorld();
					if (World != nullptr)
					{
						UNetworkingGameInstance* GI = Cast<UNetworkingGameInstance>(World->GetGameInstance());
						if (GI != nullptr)
						{
							//Empty for safety:
							GI->MapTransitionData.Empty();

							for (int i = 0; i < PlayerArray.Num(); i++)
							{
								UPlayer* Player = PlayerArray[i]->GetNetOwningPlayer();


								//ACharacterSelectPawn* SelectPawn = Cast<ACharacterSelectPawn>(PlayerArray[i]->GetPawn());
								//ACharacterSelectController* Controller = Cast<ACharacterSelectController>(Player->GetPlayerController(World));



								//APlayerState* PS = SelectPawn->GetPlayerState();


								ACharacterPlayerState* PS = Cast<ACharacterPlayerState>(PlayerArray[i]);

								//PlayerName = Machine Name String, use that!
								//GI->StoreMapTransitionData(*PS->PlayerName, SelectPawn->PlayerIndex, SelectedCharIndices[i]);

								GI->StoreMapTransitionData(*PS->GetPlayerName(), PS->GetPlayerIndex(), SelectedCharIndices[i]);

								if (Player != nullptr)
								{

									ACharacterSelectionGameMode* GM = Cast<ACharacterSelectionGameMode>(World->GetAuthGameMode());
									if (GM != nullptr)
									{
										GM->Server_ServerTravel();
									}

									////ACharacterSelectController* PC = Cast<ACharacterSelectController>(Player->GetPlayerController(World));
									//if (Controller != nullptr)
									//{
									//	Controller->Trigger_ServerTravel();
									//}

								}
								/*FUniqueNetId& ID = Cast<FUniqueNetId>(PlayerArray[i]->UniqueId);

								ACharacterSelectController* PC = Cast< ACharacterSelectController>(GetPlayerControllerFromNetId(World, ID));



								if (PC != nullptr)
								{
									PC->Trigger_ServerTravel();
								}*/


							}
						}
					}
				}
			}
		}
	}
}

bool ACharacterSelectGameState::SetPlayerNotReadyPerPlayer_Validate(int PlayerIndex)
{
	return true;
}

//Call this on left/right button trigger in the CharacterSelectPawn function
void ACharacterSelectGameState::SetPlayerNotReadyPerPlayer_Implementation(int PlayerIndex)
{
	if (Role == ROLE_Authority)
	{
		if (PlayerArray.IsValidIndex(0))
		{

			UWorld* World = GetWorld();
			if (World != nullptr)
			{
				

				for (int i = 0; i < PlayerArray.Num(); i++)
				{
					UPlayer* Player = PlayerArray[i]->GetNetOwningPlayer();
					APlayerController* Controller = Player->GetPlayerController(World);
					//ACharacterSelectPawn* SelectPawn = Cast<ACharacterSelectPawn>(PlayerArray[i]->GetPawn());

					ACharacterSelectController* ACC = Cast< ACharacterSelectController>(Controller);

					if (ACC != nullptr)
					{
						if (ACC->PlayerIndex == PlayerIndex)
						{
							if (ACC->PlayerReadyState == EPlayerReady::NonExistent)
							{
								ArePlayersReady[PlayerIndex] = EPlayerReady::NotReady;
								ACC->PlayerReadyState = EPlayerReady::NotReady;
							}
						}
					}
				}
			}
			
		}
	}
}

void ACharacterSelectGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterSelectGameState, SelectedCharIndices);

	DOREPLIFETIME(ACharacterSelectGameState, ArePlayersReady);

	DOREPLIFETIME(ACharacterSelectGameState, PlayerReadyCounter);

	DOREPLIFETIME(ACharacterSelectGameState, timerState);

	DOREPLIFETIME(ACharacterSelectGameState, timer);

	DOREPLIFETIME(ACharacterSelectGameState, isDebugModeActivated);

	DOREPLIFETIME(ACharacterSelectGameState, PerSecondTimer);

	DOREPLIFETIME(ACharacterSelectGameState, IsTransitionSoundPlayed);

	DOREPLIFETIME(ACharacterSelectGameState, IsPopUpBannerClicked);
}

bool ACharacterSelectGameState::InsertSelectedCharIntoArray_Validate(int32 PlayerIndex, int32 cNum)
{
	return true;
}

void ACharacterSelectGameState::InsertSelectedCharIntoArray_Implementation(int32 PlayerIndex, int32 cNum)
{
	if (Role == ROLE_Authority)
	{
		if (SelectedCharIndices.IsValidIndex(PlayerIndex))
		{
			SelectedCharIndices[PlayerIndex] = cNum;
		}
		else
		{
			InitializeCharIndices();
		}
	}
}

bool ACharacterSelectGameState::SetPlayerReadyInArray_Validate(int32 PlayerIndex, EPlayerReady IsReady)
{
	return true;
}

void ACharacterSelectGameState::SetPlayerReadyInArray_Implementation(int32 PlayerIndex, EPlayerReady IsReady)
{
	if (Role == ROLE_Authority)
	{
		if (ArePlayersReady.IsValidIndex(PlayerIndex))
		{
			ArePlayersReady[PlayerIndex] = IsReady;
		}
		else
		{
			InitializePlayersReady();
		}
	}

}

void ACharacterSelectGameState::Trigger_CheckReadyServerTravel()
{
	Server_CheckReadyServerTravel();
}

bool ACharacterSelectGameState::Server_CheckReadyServerTravel_Validate()
{
	return true;
}

void ACharacterSelectGameState::Server_CheckReadyServerTravel_Implementation()
{
	if (Role == ROLE_Authority)
	{
		PlayerReadyCounter = 0;
		for (int i = 0; i < ArePlayersReady.Num(); i++)
		{
			if (ArePlayersReady[i] == EPlayerReady::Ready) PlayerReadyCounter++;
		}

		if (PlayerReadyCounter >= 2
			&& timerState != ECharacterSelectTimerState::GetReady
			&& timerState != ECharacterSelectTimerState::MainCountdown
			&& timerState != ECharacterSelectTimerState::StageWiping) //This is for setting maincountdown state, not applicable to getready and stagewiping
		{
			timerState = ECharacterSelectTimerState::MainCountdown;
			if (isTesting)
			{
				timer = testing_MainCountdownMax;
			}
			else
			{
				timer = mainCountdownMax;
			}
			
			ACharacterSelectionGameMode* CSGM = Cast<ACharacterSelectionGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (CSGM != nullptr)
			{
				for (int j = 0; j < CSGM->PlayerControllerList.Num(); j++)
				{
					Cast<ACharacterSelectController>(CSGM->PlayerControllerList[j])->Server_PlayAlertAnim();
				}
			}
		}

		if ((PlayerReadyCounter == PlayerArray.Num() 
			&& timerState == ECharacterSelectTimerState::StageWiping
			&& timer <= 0)
			|| (isDebugModeActivated && ArePlayersReady[0] == EPlayerReady::Ready))
		{
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("ALL PLAYERS ARE READY!"));

				//Store transition data:
				UWorld* World = GetWorld();
				if (World != nullptr)
				{
					UNetworkingGameInstance* GI = Cast<UNetworkingGameInstance>(World->GetGameInstance());
					if (GI != nullptr)
					{
						//Empty for safety:
						GI->MapTransitionData.Empty();

						for (int i = 0; i < PlayerArray.Num(); i++)
						{
							//UPlayer* Player = PlayerArray[i]->GetNetOwningPlayer();							
							
							//ACharacterSelectPawn* SelectPawn = Cast<ACharacterSelectPawn>(PlayerArray[i]->GetPawn());
							//ACharacterSelectController* Controller = Cast<ACharacterSelectController>(UGameplayStatics::GetPlayerController(World, 0));
							//ACharacterSelectController* Controller = Cast<ACharacterSelectController>(PlayerArray[i])

							ACharacterPlayerState* PS = Cast<ACharacterPlayerState>(PlayerArray[i]);

							//PlayerName = Machine Name String, use that!
							//GI->StoreMapTransitionData(*PS->PlayerName, SelectPawn->PlayerIndex, SelectedCharIndices[i]);

							GI->StoreMapTransitionData(*PS->GetPlayerName(), PS->GetPlayerIndex(), SelectedCharIndices[i]);

						//Controller->Trigger_ServerTravel();

							ACharacterSelectionGameMode* CSGM = Cast<ACharacterSelectionGameMode>(UGameplayStatics::GetGameMode(World));
							if (CSGM != nullptr)
							{
								ACharacterSelectController* CSC = Cast<ACharacterSelectController>(CSGM->PlayerControllerList[i]);
								if (CSC != nullptr)
								{

									//CSC->InstantiateStageSwitchWipeWidget();
									//Then, remove the character select widget
									//CSC->PlayTransitionSound();
									CSC->RemoveCharacterSelectionWidget();
									CSGM->Server_ServerTravel();
								}
							}
						}
					}
					
				}

			}
		}
	}

}


bool ACharacterSelectGameState::RunTimer_Validate(float DeltaTime)
{
	return true;
}

void ACharacterSelectGameState::RunTimer_Implementation(float DeltaTime)
{
	switch (timerState)
	{
		case ECharacterSelectTimerState::WaitingForTwoPlayers:
			timer = 0;
			break;

		case ECharacterSelectTimerState::MainCountdown:
		{
			timer = -1;
			ACharacterSelectionGameMode* CSGM = Cast<ACharacterSelectionGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (CSGM != nullptr)
			{
				BlockPlayersFromJoining();

				AutoLockSelection();
				timerState = ECharacterSelectTimerState::GetReady;
				for (int j = 0; j < CSGM->PlayerControllerList.Num(); j++)
				{
					Cast<ACharacterSelectController>(CSGM->PlayerControllerList[j])->Server_PopupBanner();
				}
			}
			if (timer < 0)
			{
				

				
				


				
			}
			else
			{
				timer -= DeltaTime;
			}
			break;
		}
		case ECharacterSelectTimerState::GetReady:

			break;
		case ECharacterSelectTimerState::StageWiping:
			if (timer < 0)
			{

				//Force lock-in hovered champ/demark
				//Disable other players from joining the room
				Server_CheckReadyServerTravel();
			}
			else
			{
				timer -= DeltaTime;
				if (timer < 2.5f && !IsTransitionSoundPlayed)
				{
					ACharacterSelectionGameMode* CSGM = Cast<ACharacterSelectionGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
					if (CSGM != nullptr)
					{
						for (int i = 0; i < CSGM->PlayerControllerList.Num(); i++)
						{
							ACharacterSelectController* CSC = Cast<ACharacterSelectController>(CSGM->PlayerControllerList[i]);
							if (CSC != nullptr)
							{

								//CSC->InstantiateStageSwitchWipeWidget();
								//Then, remove the character select widget
								CSC->PlayTransitionSound();
							}
						}
						IsTransitionSoundPlayed = true;

					}
				}
			}
			break;
	}
}


bool ACharacterSelectGameState::Trigger_InstantiateStageSwitchWipeWidget_Validate()
{
	return true;
}

void ACharacterSelectGameState::Trigger_InstantiateStageSwitchWipeWidget_Implementation()
{
	if (Role == ROLE_Authority)
	{ 
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			ACharacterSelectionGameMode* CSGM = Cast<ACharacterSelectionGameMode>(UGameplayStatics::GetGameMode(World));
			if (CSGM != nullptr)
			{
				for (int i = 0; i < CSGM->PlayerControllerList.Num(); i++)
				{
					ACharacterSelectController* CSC = Cast<ACharacterSelectController>(CSGM->PlayerControllerList[i]);
					if (CSC != nullptr)
					{

						CSC->InstantiateStageSwitchWipeWidget();
					}
				}
			}
		}
	}
}

bool ACharacterSelectGameState::InitializeTimerState_Validate(ECharacterSelectTimerState state)
{
	return true;
}

void ACharacterSelectGameState::InitializeTimerState_Implementation(ECharacterSelectTimerState state)
{
	timerState = state;
}

bool ACharacterSelectGameState::AutoLockSelection_Validate()
{
	return true;
}

void ACharacterSelectGameState::AutoLockSelection_Implementation()
{
	if (Role == ROLE_Authority)
	{

		for (int i = 0; i < SelectedCharIndices.Num(); i++)
		{
			if (SelectedCharIndices[i] == -1)
			{
				SelectedCharIndices[i] = 0;
			}

			if (PlayerArray.IsValidIndex(i))
			{
				ACharacterPlayerState* CPS = Cast<ACharacterPlayerState>(PlayerArray[i]);
				if (CPS != nullptr)
				{
					CPS->SetIsPlayerReady(EPlayerReady::Ready); //set ready for every player
					ArePlayersReady[i] = EPlayerReady::Ready;
				}
			}
		}
	}
}

bool ACharacterSelectGameState::CallCountdown_Validate(float DeltaTime)
{
	return true;
}

void ACharacterSelectGameState::CallCountdown_Implementation(float DeltaTime)
{
	if (Role == ROLE_Authority)
	{
		if (timerState == ECharacterSelectTimerState::MainCountdown)
		{
			if (PerSecondTimer > 1.0f)
			{
				//Add to array for leaderboard:
				for (int i = 0; i < PlayerArray.Num(); i++)
				{
					float timerVal = 0;
					if (timer >= 0)
					{
						timerVal = FMath::FloorToFloat(timer);
						Wwise_CallCountdown(timerVal, i);
					}
				}
				//PerSecondTimer = 0;
				//To avoid network desync:
				PerSecondTimer = 1 - (timer - FMath::FloorToFloat(timer));
			}
			else
			{
				PerSecondTimer += DeltaTime;
			}
		}
	}
}



bool ACharacterSelectGameState::Wwise_CallCountdown_Validate(float cdVal, int index)
{
	return true;
}

void ACharacterSelectGameState::Wwise_CallCountdown_Implementation(float cdVal, int index)
{
	FString str = "ClockVal";

	UAkGameplayStatics::SetRTPCValue(FName(*str), cdVal, 0, PlayerArray[index]);

	UAkGameplayStatics::PostEvent(TickTock, PlayerArray[index], false, "");
}


bool ACharacterSelectGameState::InitializePopupBannerClicked_Validate()
{
	return true;
}

void ACharacterSelectGameState::InitializePopupBannerClicked_Implementation()
{
	if (Role == ROLE_Authority)
	{
		IsPopUpBannerClicked.Add(false);
		IsPopUpBannerClicked.Add(false);
		IsPopUpBannerClicked.Add(false);
		IsPopUpBannerClicked.Add(false);
		IsPopUpBannerClicked.Add(false);
		IsPopUpBannerClicked.Add(false);
	}
}

bool ACharacterSelectGameState::SetBannerReadyPerPlayer_Validate(int PlayerIndex)
{
	return true;
}

void ACharacterSelectGameState::SetBannerReadyPerPlayer_Implementation(int PlayerIndex)
{
	if (Role == ROLE_Authority)
	{
		if (IsPopUpBannerClicked.IsValidIndex(PlayerIndex))
		{
			IsPopUpBannerClicked[PlayerIndex] = true;
		}
		else
		{
			InitializePopupBannerClicked();
			IsPopUpBannerClicked[PlayerIndex] = true;
		}

		int popUpBannedClickedCounter = 0;
		for (int i = 0; i < PlayerArray.Num(); i++)
		{
			if (IsPopUpBannerClicked[i])
			{
				popUpBannedClickedCounter++;
			}
		}
		if (popUpBannedClickedCounter == PlayerArray.Num())
		{
			//Proceed to next state:
			Trigger_InstantiateStageSwitchWipeWidget();

			timer = StageWipingMax;
			timerState = ECharacterSelectTimerState::StageWiping;

			//TODO for Yassine: At this point, block anyone from joining the match:

		}
	}
}


bool ACharacterSelectGameState::SetBannerNotReadyAllPlayers_Validate()
{
	return true;
}

void ACharacterSelectGameState::SetBannerNotReadyAllPlayers_Implementation()
{
	if (Role == ROLE_Authority)
	{
		for (int i = 0; i < IsPopUpBannerClicked.Num(); i++)
		{
			IsPopUpBannerClicked[i] = false;
		}

		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			ACharacterSelectionGameMode* GM = Cast<ACharacterSelectionGameMode>(World->GetAuthGameMode());
			if (GM != nullptr)
			{
				for (int i = 0; i < GM->PlayerControllerList.Num(); i++)
				{
					ACharacterSelectController* CSC = Cast<ACharacterSelectController>(GM->PlayerControllerList[i]);
					if (CSC != nullptr)
					{
						CSC->Client_HideBanner();
					}
				}

			}
		}

		//Revert to previous timer state:
		timerState = ECharacterSelectTimerState::WaitingForTwoPlayers;

		//Unready:
		//Account for players that have left the game
		for (int i = 0; i < ArePlayersReady.Num(); i++)
		{
			if (PlayerArray.IsValidIndex(i))
			{
				ArePlayersReady[i] = EPlayerReady::NotReady;
				Cast<ACharacterPlayerState>(PlayerArray[i])->SetIsPlayerReady(EPlayerReady::NotReady);
			}
			else
			{
				ArePlayersReady[i] = EPlayerReady::NonExistent;
			}
		}
	}
}

void ACharacterSelectGameState::BlockPlayersFromJoining()
{
	//TODO: Yassine
}