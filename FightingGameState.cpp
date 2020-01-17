// Copywrite Shapeshifter 2019

#include "FightingGameState.h"
#include "StageTransitionManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "FightingGameMode.h"
#include "OnlineSessionInterface.h"
#include "OnlineSubsystemTypes.h"
#include "Engine.h"
#include "FighterController.h"
#include "FighterBase.h"
#include "AkGameplayStatics.h"
#include "CharacterCameraComp.h"


#include "NetworkingGameInstance.h"
#include "MapTransitionData.h"
#include "DemarkBase.h"
#include "MaskiumBase.h"
#include "Widget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

#include "AkAudioEvent.h"
#include "FighterSpectator.h"
#include "StageSwitchWipeWidget.h"
#include "Math/UnrealMathUtility.h"

#include "WwiseTransitionData.h"
#include "LevelMusicManager.h"
#include "AkComponent.h"

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

AFightingGameState::AFightingGameState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AFightingGameState::BeginPlay()
{
	Super::BeginPlay();

	AdjustVoiceSlider();

	SpawnLevelMusicManager();

	//Make below local, apparently posting from levelmusicmanager causes weird behavior, use below instead:

	//Initialization:

	BackgroundMusicComp = UAkGameplayStatics::SpawnAkComponentAtLocation(GetWorld(), StageEvent, nullptr, FVector(0, 0, 0), FRotator(0, 0, 0), true, "", "", false);
	if (BackgroundMusicComp != nullptr)
	{
		AdjustBackgroundMusicCompVol();
		SetMusicStateOnNumOfPlayers();
	}
	
}

void AFightingGameState::AdjustBackgroundMusicCompVol()
{
	UNetworkingGameInstance* NGI = GetGameInstance<UNetworkingGameInstance>();

	if (NGI != nullptr)
	{
		BackgroundMusicComp->SetOutputBusVolume(NGI->MusSliderVal);
	}
}

// Called every frame
void AFightingGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Timer:
	if (HasAuthority()) //To ensure that the client has rights to call the RunTimer(DeltaTime) function:
	{
		RunTimer(DeltaTime);
	}
	TickBasedOnState();
}

void AFightingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFightingGameState, timerState);
	DOREPLIFETIME(AFightingGameState, timer);

	DOREPLIFETIME(AFightingGameState, Stage2StartingPoints);
	DOREPLIFETIME(AFightingGameState, Stage3StartingPoints);
	DOREPLIFETIME(AFightingGameState, Stage4StartingPoints);
	DOREPLIFETIME(AFightingGameState, Stage5StartingPoints);
	DOREPLIFETIME(AFightingGameState, currentStage);

	DOREPLIFETIME(AFightingGameState, numOfSurvivingPlayers);
	DOREPLIFETIME(AFightingGameState, survivingPlayers);

	DOREPLIFETIME(AFightingGameState, numOfSelectionScreenPlayers);
	DOREPLIFETIME(AFightingGameState, hasGameStarted);

	DOREPLIFETIME(AFightingGameState, isNumPlayerSet);
	DOREPLIFETIME(AFightingGameState, Stage1StartingPoints);
	DOREPLIFETIME(AFightingGameState, isGameInitialized);
	DOREPLIFETIME(AFightingGameState, isCharSetCounter);

	DOREPLIFETIME(AFightingGameState, activateDevMode);
	DOREPLIFETIME(AFightingGameState, charToTest);

	DOREPLIFETIME(AFightingGameState, FinalStageDataArr);
	DOREPLIFETIME(AFightingGameState, SpawnedPawns);
	DOREPLIFETIME(AFightingGameState, CharColour);
	DOREPLIFETIME(AFightingGameState, WinningPlayer);

	DOREPLIFETIME(AFightingGameState, PerSecondTimer);

	DOREPLIFETIME(AFightingGameState, bTransitionFullyDisappeared);
	DOREPLIFETIME(AFightingGameState, bIsBackgroundMusicPlayed);
	DOREPLIFETIME(AFightingGameState, testVol);

	DOREPLIFETIME(AFightingGameState, Loser);
	DOREPLIFETIME(AFightingGameState, ElevatorWipeStopTime);
	DOREPLIFETIME(AFightingGameState, ElevatorWipe_HasWipeInstantiatedFadedIn);
	DOREPLIFETIME(AFightingGameState, ElevatorWipe_HasWipeStopFinished);

	DOREPLIFETIME(AFightingGameState, hasBeenInElevator);
	DOREPLIFETIME(AFightingGameState, hasGotOffElevator);

	DOREPLIFETIME(AFightingGameState, isDevModeScreenWipeDisabled);

	DOREPLIFETIME(AFightingGameState, LevelMusicManager);
	DOREPLIFETIME(AFightingGameState, IsThirtySecRTPCSet);
	DOREPLIFETIME(AFightingGameState, IsZeroSecRTPCSet);

	DOREPLIFETIME(AFightingGameState, Wwise_PlayerRanking);
	DOREPLIFETIME(AFightingGameState, ElevatorWipe_IsDelayFinished);
	DOREPLIFETIME(AFightingGameState, numOfStageCompleteScreensInstantiated);
	DOREPLIFETIME(AFightingGameState, numOfStageCompleteScreensAnimsPlayed);
	DOREPLIFETIME(AFightingGameState, IsLoserDetected);

	DOREPLIFETIME(AFightingGameState, HasStageChangeUIInstantiated);
	DOREPLIFETIME(AFightingGameState, awaitingToDelete);
	DOREPLIFETIME(AFightingGameState, IsCharDieSoundPlayed);

}


bool AFightingGameState::RunTimer_Validate(float DeltaTime)
{
	return true;
}

void AFightingGameState::RunTimer_Implementation(float DeltaTime)
{
	if (Role == ROLE_Authority)
	{

		CallCountdown(DeltaTime);

		switch (timerState)
		{
		case ETimerState::InitialCountdown:
		{
			if (timer <= 0)
			{
				//Reinitialize:
				IsCharDieSoundPlayed = false;

				//Re-initialize stage change ui...
				HasStageChangeUIInstantiated = false;

				//Instantiate mini-leaderboard widget:
				const AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (FGM != nullptr)
				{
					for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
					{
						AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);
						if (FC != nullptr)
						{
							FC->InstantiateMiniLeaderboardWidget();
						}
					}
				}
			}

			if (currentStage == 5)
			{
				if (timer <= 0)
				{

					//Edge case: If 1 player:=======================================================

					//Insta-win:
					AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

					if (survivingPlayers.IsValidIndex(0) && FGM->numOfSelectionScreenPlayers == 1)
					{
						survivingPlayers[0]->Condition = ECondition::Win;
						survivingPlayers[0]->bIsWinner = true;
						survivingPlayers[0]->CreateWinLoseWidget(survivingPlayers[0]->Condition);
						timerState = ETimerState::WinnerDecided;
						timer = 3;
						TogglePawnInput(false);
					}

					//===============================================================================


					timerState = ETimerState::FinalStage;
					TogglePawnInput(true);
				}
				else
				{
					PlayRoofMusic();
					timer -= DeltaTime;
					TogglePawnInput(false);
				}
			}
			else
			{
				if (timer <= 0)
				{
					timerState = ETimerState::MainCountdown;
					timer = mainTimerMax;

					TogglePawnInput(true);
				}
				else
				{
					timer -= DeltaTime;
					TogglePawnInput(false);
				}
			}
		}
		break;

		case ETimerState::MainCountdown:
			if (timer <= 0)
			{
				if (!IsCharDieSoundPlayed)
				{
					PostCharDieEvent();
					IsCharDieSoundPlayed = true;
				}

				//Destroy mini-leaderboard widget:
				const AFightingGameMode* FGM = GetDefaultGameMode<AFightingGameMode>();
				if (FGM != nullptr)
				{
					for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
					{
						AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);
						if (FC != nullptr)
						{
							FC->DestroyMiniLeaderboardWidget();
						}
					}
				}

				TogglePawnInput(false);

				timer = deadPlayerEliminationTimerMax;

				if (currentStage >= 1 && currentStage <= 4)
				{
					if (survivingPlayers.Num() > 2)
					{
						if (hasBeenInElevator == false)
						{
							EliminatePlayerNormalStage();
							//timerState = ETimerState::Elevator;

							Trigger_StopAllAnims(); //Stop All animations

							//Execute Dead Player Must be done before the state triggers...
							ExecuteDeadPlayerEliminationSequence();

							//Note: Timer counts upwards for elevator states!
							timer = 0;
						}
					}
				}
			}
			else
			{
				timer -= DeltaTime;
				if (!IsLoserDetected && Loser == nullptr)
				{
					DetectAssignLoser(); //Update the healthbar black...
				}
				SetCountdownRankingState(timer);

				numOfStageCompleteScreensInstantiated = 0;
				numOfStageCompleteScreensAnimsPlayed = 0;
			}
			break;

		case ETimerState::DeadPlayerElimination:
			if (timer > deadPlayerEliminationTimerMax)
			{
				DeletePlayer();
				timer = 0;
				timerState = ETimerState::Elevator_TransitionIn;
			}
			else
			{
				timer += DeltaTime;
				TogglePawnInput(false);
			}
			break;

		case ETimerState::Elevator_TransitionIn:

			ElevatorWipe(ETimerState::Elevator, DeltaTime);
			break;

		case ETimerState::Elevator:
		{
			SurvivingPlayers_SetFullHealth();

			TArray<AActor*> testArray;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFighterBase::StaticClass(), testArray);
			for (int32 i = 0; i < testArray.Num(); i++)
			{
				if (testArray[i] != nullptr)
				{
					AFighterBase* fighter = Cast<AFighterBase>(testArray[i]);
					if (fighter != nullptr)
					{
						UCharacterCameraComp* cam = Cast<UCharacterCameraComp>(fighter->GetComponentByClass(UCharacterCameraComp::StaticClass()));
						if (cam != nullptr)
						{
							cam->SetCamState(ECameraStateEnum::ElevatorState);
						}
					}
				}
			}
			timer2 += 1;

			//Have fun in the elevator ;)
			TogglePawnInput(true);

			if (timer2 >= WidgetSpawnTime)
			{
				if (!HasStageChangeUIInstantiated)
				{
					InstantiateStageChangeUI();
					HasStageChangeUIInstantiated = true;
				}
			}

			if (timer2 >= howLongYouStayInElevator)
			{
				//goto state: Elevator_TransitionOut
				timerState = ETimerState::Elevator_TransitionOut;
			}

			break;
		}

		case ETimerState::Elevator_TransitionOut:
		{
			SurvivingPlayers_SetFullHealth();

			ElevatorWipe(ETimerState::InitialCountdown, DeltaTime);
			TArray<AActor*> testArray;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFighterBase::StaticClass(), testArray);
			for (int32 i = 0; i < testArray.Num(); i++)
			{
				if (testArray[i] != nullptr)
				{
					AFighterBase* fighter = Cast<AFighterBase>(testArray[i]);
					if (fighter != nullptr)
					{
						UCharacterCameraComp* cam = Cast<UCharacterCameraComp>(fighter->GetComponentByClass(UCharacterCameraComp::StaticClass()));
						if (cam != nullptr)
						{
							cam->SetCamState(ECameraStateEnum::GameplayState);
						}
					}
				}
			}
			break;
		}

		case ETimerState::FinalStage:
			//If not win/lose yet:

			break;

		case ETimerState::WinnerDecided:
			if (timer < 0)
			{
				timerState = ETimerState::ServerTravelling;
				//Kickout
				TeleportPlayersToLeaderboardMap();
			}
			else
			{
				timer -= DeltaTime;
				TogglePawnInput(false);
			}
			break;

		case ETimerState::ServerTravelling:
			//Do nothing!

			break;

		default:
			break;

		}
	}
}

bool AFightingGameState::TeleportPlayersToStage_Validate(int stageNum, bool isJustStarted)
{
	return true;
}

void AFightingGameState::TeleportPlayersToStage_Implementation(int stageNum, bool isJustStarted)
{
	if (Role == ROLE_Authority)
	{
		bool areStartingPointsFound = false;
		TArray<AActor*> StageStartingPts;

		switch (stageNum)
		{
		case 2:
			StageStartingPts = Stage2StartingPoints;
			UAkGameplayStatics::SetState("Game_State", "Street");
			//PlayMusic()
			break;

		case 3:
			StageStartingPts = Stage3StartingPoints;
			UAkGameplayStatics::SetState("Game_State", "Street");
			break;

		case 4:
			StageStartingPts = Stage4StartingPoints;
			UAkGameplayStatics::SetState("Game_State", "Street");
			break;

		case 5:
			StageStartingPts = Stage5StartingPoints;
			UAkGameplayStatics::SetState("Game_State", "Roof");
			break;
		case 6:
			StageStartingPts = StageEStartingPoints;
			//UAkGameplayStatics::SetState("Game_State", "Stage5");
			break;
		}

		if (StageStartingPts.Num() > 0)
		{
			for (int i = 0; i < survivingPlayers.Num(); i++)
			{
				survivingPlayers[i]->SetActorLocation(StageStartingPts[i]->GetActorLocation());
			}

			//Add a parameter to this function later: Is initial
			if (!isJustStarted)  //Need to set start first...
			{
				timerState = ETimerState::InitialCountdown;
				timer = initialTimerMax;
				TogglePawnInput(false);
			}
		}
		else
		{
			if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "Stage1")
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("End of Level.")));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You are not in stage 1. Get yo ass to stage 1 for stage transitions.")));
				//TogglePawnInput(true);
			}
		}
	}
}

bool AFightingGameState::SetStartingPoints_Validate()
{
	return true;
}

void AFightingGameState::SetStartingPoints_Implementation()
{
	if (Role == ROLE_Authority)
	{
		FString stage2Tag = "Stage2";
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*stage2Tag), Stage2StartingPoints);

		FString stage3Tag = "Stage3";
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*stage3Tag), Stage3StartingPoints);

		FString stage4Tag = "Stage4";
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*stage4Tag), Stage4StartingPoints);

		FString stage5Tag = "Stage5";
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*stage5Tag), Stage5StartingPoints);

		FString stageETag = "StageE";
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*stageETag), StageEStartingPoints);
	}
}

bool AFightingGameState::TogglePawnInput_Validate(bool setEnabled)
{
	return true;
}

void AFightingGameState::TogglePawnInput_Implementation(bool setEnabled)
{
	//Both of the below causes replication problems:
	//if (Role == ROLE_Authority)
		//if (HasAuthority())
	//{
	//Multicast, since fighting movements are client-based
	if (setEnabled)
	{
		for (int i = 0; i < survivingPlayers.Num(); i++)
		{
			if (survivingPlayers[i] != nullptr)
			{
				survivingPlayers[i]->bIsMovementEnabled = true;
			}
		}
	}
	else
	{
		for (int i = 0; i < survivingPlayers.Num(); i++)
		{
			if (survivingPlayers[i] != nullptr)
			{
				survivingPlayers[i]->bIsMovementEnabled = false;
			}
		}
	}
	//}
}

bool AFightingGameState::AddToSurvivingPlayers_Validate()
{
	return true;
}

void AFightingGameState::AddToSurvivingPlayers_Implementation()
{
	if (Role == ROLE_Authority)
	{
		survivingPlayers.Empty();
		for (int i = 0; i < PlayerArray.Num(); i++)
		{
			AFighterBase* FB = Cast<AFighterBase>(PlayerArray[i]->GetPawn());
			if (FB != nullptr)
			{
				survivingPlayers.Add(FB);
			}
			else
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Null Pawn in Player State grabbed from player array!"));
			}
		}
	}
}

bool AFightingGameState::EliminatePlayerNormalStage_Validate()
{
	return true;
}

void AFightingGameState::EliminatePlayerNormalStage_Implementation()
{
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			//Find him!
			//BUG: Aggression is too high
			//SOLUTION: Store the player to eliminate as a variable (the first one with lowest health)
			//			Then, eliminate him
			//If everyone has higher than 0 health, eliminate the lowest health one


			//if nobody died:
			if (Loser == nullptr)
			{
				float lowestHealth = 9999999999.0f;
				int lowestHealthPlayerIndex = -1;
				for (int i = 0; i < survivingPlayers.Num(); i++)
				{
					if (Cast<AFighterBase>(survivingPlayers[i])->GetHealth() < lowestHealth)
					{
						lowestHealth = Cast<AFighterBase>(survivingPlayers[i])->GetHealth();
						lowestHealthPlayerIndex = i;
					}
				}

				Loser = survivingPlayers[lowestHealthPlayerIndex];
				Loser->SetHealth(0);
			}
		}
	}
}

bool AFightingGameState::StartGame_Validate()
{
	return true;
}

void AFightingGameState::StartGame_Implementation()
{
	if (Role == ROLE_Authority)
	{
		if (survivingPlayers.Num() == PlayerArray.Num()) //checks whether every player is ready
		{
			if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "Stage1") //Timer only exists in map 'Stage1'
			{
				if (timerState == ETimerState::EndTransitionCompleted) //To prevent the later client to reset the state to initial cd when game's running
				{

					//Spawn Timer Widgets:
					AFightingGameMode* GM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
					if (GM != nullptr)
					{
						for (int i = 0; i < GM->PlayerControllerList.Num(); i++)
						{
							AFighterController* FC = Cast<AFighterController>(GM->PlayerControllerList[i]);
							if (FC != nullptr)
							{
								//FC->InstantiateTimerScreen();
							}
						}
					}

					//PlayMusic

					//Tackle edge cases here:
					//Set the timer and timerstate here:

					if (LevelMusicManager != nullptr)
					{
						Trigger_InitializeMusicSlider(currentStage);
					}
					timerState = ETimerState::InitialCountdown;
					timer = initialTimerMax;
					TogglePawnInput(false);
					hasGameStarted = true;
					//}
				}
			}
		}
	}
}

bool AFightingGameState::Trigger_InitializeMusicSlider_Validate(int stageNum)
{
	return true;
}

void AFightingGameState::Trigger_InitializeMusicSlider_Implementation(int stageNum)
{
	if (GetNetMode() == ENetMode::NM_Client || GetNetMode() == ENetMode::NM_ListenServer)
	{
		LevelMusicManager->AdjustMusicSlider(stageNum, true);
	}
}

bool AFightingGameState::SpawnLevelMusicManager_Validate()
{
	return true;
}



void AFightingGameState::SpawnLevelMusicManager_Implementation()
{
	if (GetNetMode() == ENetMode::NM_Client || GetNetMode() == ENetMode::NM_ListenServer)
	{
		FActorSpawnParameters SpawnInfo;
		LevelMusicManager = GetWorld()->SpawnActor<ALevelMusicManager>(LevelMusicManagerClass, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnInfo);
	}
}

bool AFightingGameState::RetrieveTransitionData_Validate()
{
	return true;
}

void AFightingGameState::RetrieveTransitionData_Implementation()
{
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			UNetworkingGameInstance* GI = Cast<UNetworkingGameInstance>(World->GetGameInstance());
			if (GI != nullptr)
			{
				AFightingGameMode* GM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(World));
				if (GM->isEveryPlayerLoaded)
				{
					isGameInitialized = true;
					timerState = ETimerState::SetPawnPosition;
				}
			}
		}
	}
}

bool AFightingGameState::TickBasedOnState_Validate()
{
	return true;
}

void AFightingGameState::TickBasedOnState_Implementation()
{
	if (Role == ROLE_Authority && HasAuthority())
	{
		if (!isGameInitialized)
		{
			timerState = ETimerState::Initialization;
			RetrieveTransitionData();
		}
		else
		{
			if (timerState == ETimerState::SetPawnPosition)
			{
				SetPawnPos();
			}
			else if (timerState == ETimerState::EndTransition) //dev mode breaks transition screen
			{
				EndTransition();
			}
			else
			{
				if (!hasGameStarted) {
					StartGame();
				}
			}
		}
	}
}


bool AFightingGameState::InitializeFinalStageData_Validate()
{
	return true;
}

void AFightingGameState::InitializeFinalStageData_Implementation()
{
	if (Role == ROLE_Authority)
	{
		for (int i = 0; i < PlayerArray.Num(); i++)
		{
			if (survivingPlayers.Contains(Cast<AFighterBase>(PlayerArray[i]->GetPawn())))
			{
				FFinalStageData data;
				data.SetCondition(ECondition::Undecided);
				data.SetPlayerIndex(Cast<AFighterPlayerState>(PlayerArray[i])->PlayerIndex);
				FinalStageDataArr.Add(data);
			}
		}
	}
}

bool AFightingGameState::OnZeroHealthDecideWinLose_Validate(AFighterBase* fb)
{
	return true;
}

void AFightingGameState::OnZeroHealthDecideWinLose_Implementation(AFighterBase* fb)
{

	if (Role == ROLE_Authority)
	{
		if (timerState == ETimerState::FinalStage)
		{

			SpawnFirework();


			for (int i = 0; i < survivingPlayers.Num(); i++)
			{
				if (survivingPlayers[i] == fb)
				{
					survivingPlayers[i]->Condition = ECondition::Lose;
					survivingPlayers[i]->bIsDead = true;
					survivingPlayers[i]->CreateWinLoseWidget(survivingPlayers[i]->Condition);

					//Add to array for leaderboard:
					PushStoreLeaderboardData(survivingPlayers[i]);
				}
				else
				{
					survivingPlayers[i]->Condition = ECondition::Win;
					survivingPlayers[i]->bIsWinner = true;
					survivingPlayers[i]->CreateWinLoseWidget(survivingPlayers[i]->Condition);

					PostGameOverSound();
					PostCharDieEvent();
					survivingPlayers[i]->GetController<AFighterController>()->PlayNovaRoyaleSound();

					WinningPlayer = survivingPlayers[i];
				}
			}
			timerState = ETimerState::WinnerDecided;
			timer = 10;
			TogglePawnInput(false);

			//Stop all Anims in the end:
			Trigger_StopAllAnims();
		}
	}
}


bool AFightingGameState::TeleportPlayersToLeaderboardMap_Validate()
{
	return true;
}

void AFightingGameState::TeleportPlayersToLeaderboardMap_Implementation()
{
	if (Role == ROLE_Authority)
	{	
		//Edge case: 1 player only...
		if (WinningPlayer == nullptr)
		{
			WinningPlayer = Cast<AFighterBase>(PlayerArray[0]->GetPawn());
		}
		//Store Winner to LeaderboardDataArray:
		PushStoreLeaderboardData(WinningPlayer);

		//Open map:
		for (int i = 0; i < PlayerArray.Num(); i++)
		{

			//Remove widgets:
			TArray<UUserWidget*> widgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), widgets, UUserWidget::StaticClass(), true);
			for (int j = 0; j < widgets.Num(); j++)
			{
				//widgets[i]->Destruct();
				widgets[j]->RemoveFromParent();
			}

			//Initialize transition widget:
			AFighterController* FC = Cast<AFighterController>(PlayerArray[i]->GetOwner());
			if (FC != nullptr)
			{
				FC->InstantiateStageSwitchWipeWidget(true, false);
				FC->Trigger_WidgetOnTickWipe();
			}

			AFightingGameMode* GM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM != nullptr)
			{
				GM->Trigger_ServerTravel();
			}
		}

	}
}

bool AFightingGameState::PushStoreLeaderboardData_Validate(AFighterBase* fighter)
{
	return true;
}

void AFightingGameState::PushStoreLeaderboardData_Implementation(AFighterBase* fighter)
{
	if (Role == ROLE_Authority)
	{
		//Add to array for leaderboard:
		UNetworkingGameInstance* NGI = Cast<UNetworkingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (NGI != nullptr)
		{
			AFighterPlayerState* PS = Cast<AFighterPlayerState>(fighter->GetPlayerState());
			if (PS)
			{
				if (Cast<ADemarkBase>(fighter) != nullptr)
				{
					NGI->StoreLeaderboardData(PS->GetPlayerName(), PS->PlayerIndex, 0, fighter->GetAggression());
				}
				else if (Cast<AMaskiumBase>(fighter) != nullptr)
				{
					NGI->StoreLeaderboardData(PS->GetPlayerName(), PS->PlayerIndex, 1, fighter->GetAggression());
				}
				else
				{
					//TODO: Maskium and other characters
				}
			}
		}
	}
}

bool AFightingGameState::CallCountdown_Validate(float DeltaTime)
{
	return true;
}


void AFightingGameState::CallCountdown_Implementation(float DeltaTime)
{
	if (Role == ROLE_Authority)
	{
		if (PerSecondTimer > 1.0f)
		{

			float timerVal = 0;
			if ((timer > 0.99f && timerState == ETimerState::MainCountdown) || ((timer <= 3 || timer >= 0.0f) && timerState == ETimerState::InitialCountdown))
			{
				timerVal = FMath::FloorToFloat(timer);

				if (timerState == ETimerState::MainCountdown)
				{
					PostTimerSound(timerVal);
				}
				else if (timerState == ETimerState::InitialCountdown)
				{
					PostInitialTimerSound(timerVal);
				}
			}

			//To avoid network desync:
			PerSecondTimer = 1 - (timer - FMath::FloorToFloat(timer));
		}
		else
		{
			PerSecondTimer += DeltaTime;
		}
	}
}


bool AFightingGameState::PostTimerSound_Validate(float cdVal)
{
	return true;
}

void AFightingGameState::PostTimerSound_Implementation(float cdVal)
{
	FString str = "ClockVal";

	UAkGameplayStatics::SetRTPCValue(FName(*str), cdVal, 0, this);

	UAkGameplayStatics::PostEvent(announcer, this, false, "");

}

bool AFightingGameState::PostInitialTimerSound_Validate(float cdVal)
{
	return true;
}

void AFightingGameState::PostInitialTimerSound_Implementation(float cdVal)
{
	FString str = "ClockVal";

	if (cdVal > 3) cdVal = 3;

	UAkGameplayStatics::SetRTPCValue(FName(*str), cdVal, 0, this);

	UAkGameplayStatics::PostEvent(initalAnnouncer, this, false, "");

}

bool AFightingGameState::PostGameOverSound_Validate()
{
	return true;
}

void AFightingGameState::PostGameOverSound_Implementation()
{

	UAkGameplayStatics::PostEvent(gameOverSound, this, false, "");

}


bool AFightingGameState::PostCharDieEvent_Validate()
{
	return true;
}

void AFightingGameState::PostCharDieEvent_Implementation()
{

	UAkGameplayStatics::PostEvent(CharDieEvent, this, false, "");

	//timer = 0 sec event:

	FString str = "ClockVal";

	UAkGameplayStatics::SetRTPCValue(FName(*str), 0, 0, this);

	UAkGameplayStatics::PostEvent(announcer, this, false, "");

}

bool AFightingGameState::EndTransition_Validate()
{
	return true;
}


void AFightingGameState::EndTransition_Implementation()
{
	if (Role == ROLE_Authority)
	{
		//check whether each controller successfully possesses a pawn
		//If survivingplayers == num in player array
		int PawnPossessedCounter = 0;
		for (int i = 0; i < PlayerArray.Num(); i++)
		{
			if (PlayerArray[i]->GetPawn() != nullptr)
			{
				PawnPossessedCounter++;
			}
		}

		if (!activateDevMode)
		{
			if (PawnPossessedCounter == survivingPlayers.Num() && !bTransitionFullyDisappeared)
			{
				AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (FGM != nullptr)
				{
					int WipedCounter = 0;

					for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
					{
						AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);

						if (FC != nullptr)
						{
							if (!FC->isStageSwitchWipeWidgetNull)
							{
								if (!FC->hasWiped)
								{
									//comment this out first:
									FC->WipeTransitionScreen(false);
								}
								else
								{
									WipedCounter++;
									FC->DestroyWipeWidget();
								}
							}
						}
					}

					if (WipedCounter == PawnPossessedCounter)
					{
						timerState = ETimerState::EndTransitionCompleted;
					}
				}
			}
		}
		else
		{
			//cuz wipe widget in dev mode gives weird-ass crashes
			TArray<UUserWidget*> WipeWidgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), WipeWidgets, UStageSwitchWipeWidget::StaticClass(), true);
			for (int j = 0; j < WipeWidgets.Num(); j++)
			{
				//widgets[i]->Destruct();
				WipeWidgets[j]->RemoveFromParent();
			}
			timerState = ETimerState::EndTransitionCompleted;
		}
	}
}

bool AFightingGameState::SetPawnPos_Validate()
{
	return true;
}

//To avoid the spawn bug
void AFightingGameState::SetPawnPos_Implementation()
{
	if (Role == ROLE_Authority)
	{
		SetStartingPoints();

		if (survivingPlayers.Num() != PlayerArray.Num())
		{
			AddToSurvivingPlayers(); //Assume every player 

		//TP players before the end game transition gets called
			AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (FGM->numOfSelectionScreenPlayers == 1)
			{
				currentStage = 5;
				TeleportPlayersToStage(5, true);
				PlayRoofMusic();
			}
			else if (FGM->numOfSelectionScreenPlayers == 2)
			{
				currentStage = 5;
				TeleportPlayersToStage(5, true);
				PlayRoofMusic();
			}
			else
			{
				currentStage = 1;
				UAkGameplayStatics::SetState("Game_State", "Street");
			}

		}
		else
		{
			const AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			timerState = ETimerState::EndTransition;
		}
	}
}

void AFightingGameState::AdjustVoiceSlider()
{
	UNetworkingGameInstance* NGI = GetGameInstance<UNetworkingGameInstance>();
	if (NGI != nullptr)
	{
		UAkGameplayStatics::SetOutputBusVolume(NGI->VoiceSliderVal, this);
	}
}

bool AFightingGameState::DetectAssignLoser_Validate()
{
	return true;
}

void AFightingGameState::DetectAssignLoser_Implementation()
{
	if (Role == ROLE_Authority)
	{
		if (Loser == nullptr)
		{
			for (int i = 0; i < survivingPlayers.Num(); i++)
			{
				if (survivingPlayers[i]->GetHealth() <= 0)
				{
					Loser = survivingPlayers[i];
					timer = 0.02f;
					IsLoserDetected = true;
				}
			}
		}
	}
}

bool AFightingGameState::SurvivingPlayers_SetFullHealth_Validate()
{
	return true;
}

void AFightingGameState::SurvivingPlayers_SetFullHealth_Implementation()
{
	if (Role == ROLE_Authority)
	{
		for (int i = 0; i < survivingPlayers.Num(); i++)
		{
			survivingPlayers[i]->SetHealth(100);
			survivingPlayers[i]->bIsDead = false;
		}
	}
}


bool AFightingGameState::StageMusicTransition_Validate()
{
	return true;
}

void AFightingGameState::StageMusicTransition_Implementation()
{
	//Stop and play music:
	if (LevelMusicManager != nullptr)
	{
		LevelMusicManager->AdjustMusicSlider(currentStage, false);
	}
}

bool AFightingGameState::ElevatorWipe_Validate(ETimerState nextState, float DeltaTime)
{
	return true;
}

void AFightingGameState::ElevatorWipe_Implementation(ETimerState nextState, float DeltaTime)
{
	AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (FGM != nullptr)
	{
		int WipedCounter = 0;

		//1. Instantiate and fade in:
		if (!ElevatorWipe_HasWipeInstantiatedFadedIn
			&& WipedCounter < FGM->PlayerControllerList.Num())
		{

			for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
			{
				AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);
				if (FC != nullptr)
				{
					FC->InstantiateStageSwitchWipeWidget(true, false);
					WipedCounter++;
				}
			}

			//Set fade-in Wwise state:

			if (GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Client)
			{
				if (LevelMusicManager != nullptr && timerState == ETimerState::Elevator_TransitionIn)
				{
					LevelMusicManager->AdjustMusicSlider(20, false);
				}
			}


			PlayElevatorTransitionSound();

			ElevatorWipe_HasWipeInstantiatedFadedIn = true;
		}

		//1.5: Delay between transition for the transition screen to slide in before teleporting:
		if (ElevatorWipe_HasWipeInstantiatedFadedIn
			&& !ElevatorWipe_IsDelayFinished)
		{

			if (timer > transitionInSlightDelayMax)
			{
				ElevatorWipe_IsDelayFinished = true;
				timer = 0;
			}
			else
			{
				timer += DeltaTime;
			}
		}

		//2. Delay, meanwhile teleport them to the elevator/designated stages(transition out):
		if (ElevatorWipe_IsDelayFinished
			&& !ElevatorWipe_HasWipeStopFinished)
		{
			//delay timer:
			if (timer > ElevatorWipeStopTime)
			{
				ElevatorWipe_HasWipeStopFinished = true;
				timer = 0;
			}
			else
			{
				timer += DeltaTime;
				if (timerState == ETimerState::Elevator_TransitionIn)
				{
					TogglePawnInput(true);
				}
				else if (timerState == ETimerState::Elevator_TransitionOut)
				{
					TogglePawnInput(false);
				}
			}

			//Braden:=============================
			if (timerState == ETimerState::Elevator_TransitionIn)
			{
				if (hasBeenInElevator == false)
				{
					TeleportPlayersToStage(6, true);
					hasBeenInElevator = true;
				}

				//InstantiateSpectatorWidget:
				Trigger_InstantiateSpectatorWidget();
			}
			else if (timerState == ETimerState::Elevator_TransitionOut)
			{
				//transitionOut

				if (hasGotOffElevator == false)
				{
					if (survivingPlayers.Num() > 2)
					{
						TeleportPlayersToStage(3, true);
						currentStage += 1;
					}
					else if (survivingPlayers.Num() == 2)
					{
						TeleportPlayersToStage(5, true);
						currentStage = 5;
					}
					hasGotOffElevator = true;
				}
			}
			//=====================================
		}

		int wipeFinishedCounter = 0;

		//3. Fade out and destroy:
		if (ElevatorWipe_HasWipeInstantiatedFadedIn
			&& ElevatorWipe_HasWipeStopFinished)
		{

			for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
			{
				AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);
				if (FC != nullptr)
				{
					if (!FC->isStageSwitchWipeWidgetNull)
					{
						if (!FC->hasWiped)
						{
							FC->WipeTransitionScreen(false);
						}
						else
						{
							wipeFinishedCounter++;
							FC->DestroyWipeWidget();
						}
					}
				}
			}

			//change state
			if (LevelMusicManager != nullptr && timerState == ETimerState::Elevator_TransitionOut)
			{
				if (GetNetMode() == ENetMode::NM_Client || GetNetMode() == ENetMode::NM_ListenServer)
				{
					LevelMusicManager->AdjustMusicSlider(currentStage, false);
				}
				//Potential band-aid:...
				if (currentStage == 5)
				{
					PlayRoofMusic();
				}
			}
		}

		if (wipeFinishedCounter == FGM->PlayerControllerList.Num())
		{
			//Re-initialize variables...
			ElevatorWipe_HasWipeInstantiatedFadedIn = false;
			ElevatorWipe_IsDelayFinished = false;
			ElevatorWipe_HasWipeStopFinished = false;
			hasBeenInElevator = false;
			hasGotOffElevator = false;
			timer2 = 0;
			numOfStageCompleteScreensInstantiated = 0;
			numOfStageCompleteScreensAnimsPlayed = 0;
			Loser = nullptr;
			IsLoserDetected = false;


			if (nextState == ETimerState::InitialCountdown)
			{
				//Stop all Anims before next stage:
				Trigger_StopAllAnims();

				timer = initialTimerMax;
				timerState = nextState;
				TogglePawnInput(false);

				//Re-initialize...
				IsThirtySecRTPCSet = false;
				IsZeroSecRTPCSet = false;

				Loser = nullptr;
			}
			else
			{
				timer = 0;
				timerState = nextState;
				//Have fun in the elevator ;)
				TogglePawnInput(true);
			}
		}
	}
}

bool AFightingGameState::ExecuteDeadPlayerEliminationSequence_Validate()
{
	return true;
}

void AFightingGameState::ExecuteDeadPlayerEliminationSequence_Implementation()
{
	if (Role == ROLE_Authority)
	{


		//Need to check whether every stage complete UI is instantiated before repossessing... server is much faster than clients

		AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (FGM != nullptr)
		{
			//Play Death Animation()

			//Make sure the screens doesn't spawn in runtime, just once...
			if (numOfStageCompleteScreensInstantiated < FGM->PlayerControllerList.Num())
			{
				InstantiateDeadPlayerUI();
			}

			//variable below is incremented by widget:
			if (numOfStageCompleteScreensAnimsPlayed >= FGM->PlayerControllerList.Num())
			{
				PossessSpectatorPawnAndDeletePlayer();
			}
		}
	}
}

bool AFightingGameState::PossessSpectatorPawnAndDeletePlayer_Validate()
{
	return true;
}

void AFightingGameState::PossessSpectatorPawnAndDeletePlayer_Implementation()
{
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();

		if (World != nullptr)
		{

			awaitingToDelete = Loser;

			//Add to array for leaderboard:
			PushStoreLeaderboardData(awaitingToDelete);
			AFighterController* FC = Cast<AFighterController>(awaitingToDelete->GetController());

			if (FC != nullptr)
			{
				//TODO: Move these to player elimination state:
				//Possess spectator pawn after creation:
				FActorSpawnParameters SpawnParams;
				AFighterSpectator* FS = World->SpawnActor<AFighterSpectator>(FighterSpectatorClass, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
				FC->isSpectating = true;
				FC->UnPossess();
				FC->Possess(FS);

				//survivingPlayers.RemoveAt(lowestAggressionPlayerIndex):
				if (Loser != nullptr)
				{
					survivingPlayers.Remove(Loser);
				}

				//Then, transition to next stage...
				timerState = ETimerState::DeadPlayerElimination;
			}
		}
	}
}

bool AFightingGameState::DeletePlayer_Validate()
{
	return true;
}

void AFightingGameState::DeletePlayer_Implementation()
{
	if (Role == ROLE_Authority)
	{
		//Destroy awaitingToDelete
		if (awaitingToDelete != nullptr)
		{
			awaitingToDelete->Destroy();
			awaitingToDelete = nullptr;
		}

		//re-initialize loser, and numofstagecompletescreens:
		Loser = nullptr;
	}
}

bool AFightingGameState::InstantiateDeadPlayerUI_Validate()
{
	return true;
}

void AFightingGameState::InstantiateDeadPlayerUI_Implementation()
{
	if (Role == ROLE_Authority)
	{
		AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (FGM != nullptr)
		{
			for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
			{
				AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);
				if (FC != nullptr)
				{
					FC->InstantiateEliminationWidget();
					numOfStageCompleteScreensInstantiated++;
				}
			}
		}
	}
}

bool AFightingGameState::InstantiateStageChangeUI_Validate()
{
	return true;
}

void AFightingGameState::InstantiateStageChangeUI_Implementation()
{
	if (Role == ROLE_Authority)
	{
		AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (FGM != nullptr)
		{
			for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
			{
				AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);
				if (FC != nullptr)
				{
					FC->InstantiateStageChangeWidget();
				}
			}
		}
	}
}

bool AFightingGameState::SetCountdownRankingState_Validate(float cdtimer)
{
	return true;
}

void AFightingGameState::SetCountdownRankingState_Implementation(float cdtimer)
{
	bool shouldSetState = false;
	if (cdtimer < 31 && !IsThirtySecRTPCSet)
	{
		shouldSetState = true;
		IsThirtySecRTPCSet = true;
	}
	else if (cdtimer < 1 && !IsZeroSecRTPCSet)
	{
		shouldSetState = true;
		IsThirtySecRTPCSet = true;
	}

	if (shouldSetState)
	{
		if (Wwise_PlayerRanking == survivingPlayers.Num() - 1)
		{
			UAkGameplayStatics::SetState("Player_Rating", "Losing");
		}
		else if (Wwise_PlayerRanking == 0)
		{
			UAkGameplayStatics::SetState("Player_Rating", "Winning");
		}
		else
		{
			UAkGameplayStatics::SetState("Player_Rating", "Safe");
		}
	}


}

bool AFightingGameState::PlayElevatorTransitionSound_Validate()
{
	return true;
}

void AFightingGameState::PlayElevatorTransitionSound_Implementation()
{
	if (GetNetMode() == NM_Client || GetNetMode() == NM_ListenServer)
	{
		UAkGameplayStatics::PostEvent(ElevatorTransitionEvent, this, false, "");
	}
}

bool AFightingGameState::Trigger_InstantiateSpectatorWidget_Validate()
{
	return true;
}

void AFightingGameState::Trigger_InstantiateSpectatorWidget_Implementation()
{
	if (Role == ROLE_Authority)
	{
		AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
		{
			AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);
			if (FC != nullptr)
			{
				//Spawn spector widget if it's spectator
				AFighterSpectator* FS = Cast<AFighterSpectator>(FC->GetPawn());
				if (FS != nullptr)
				{
					FS->InitializeSpectatorScreen();
				}
			}
		}
	}

}

bool AFightingGameState::PlayRoofMusic_Validate()
{
	return true;
}

void AFightingGameState::PlayRoofMusic_Implementation()
{
	UAkGameplayStatics::SetState("Game_State", "Roof");
}

bool AFightingGameState::Trigger_StopAllAnims_Validate()
{
	return true;
}

void AFightingGameState::Trigger_StopAllAnims_Implementation()
{
	AFightingGameMode* FGM = Cast<AFightingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	for (int i = 0; i < FGM->PlayerControllerList.Num(); i++)
	{
		AFighterController* FC = Cast<AFighterController>(FGM->PlayerControllerList[i]);
		if (FC != nullptr)
		{
			AFighterBase* FB = Cast<AFighterBase>(FC->GetPawn());
			if (FB != nullptr)//casting != fail (spectator)
			{
				FB->Server_StopAllAnims();
			}
		}
	}
}

bool AFightingGameState::SpawnFirework_Validate()
{
	return true;
}

void AFightingGameState::SpawnFirework_Implementation()
{
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;

		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			TArray<AActor*> FireworkSpawnPoints;
			FString FireworkSpawnPointStr = "FireworkSpawnPoint";
			UGameplayStatics::GetAllActorsWithTag(World, FName(*FireworkSpawnPointStr), FireworkSpawnPoints);

			if (FireworkSpawnPoints.Num() >= 0)
			{
				FVector MiddleLocation = FireworkSpawnPoints[0]->GetActorLocation();
				FVector LeftLocation = FireworkSpawnPoints[0]->GetActorLocation() - FVector(0, 800.0f, 0);
				FVector RightLocation = FireworkSpawnPoints[0]->GetActorLocation() + FVector(0, 800.0f, 0);

				World->SpawnActor<AActor>(FireworkBP, MiddleLocation, Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, LeftLocation + FVector(10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, LeftLocation + FVector(-10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, RightLocation - FVector(10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, RightLocation + FVector(10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, RightLocation + FVector(-10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, LeftLocation - FVector(10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, LeftLocation + FVector(10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, MiddleLocation + FVector(-10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
				World->SpawnActor<AActor>(FireworkBP, MiddleLocation - FVector(10.0f, 10.0f, 10.0f), Rotation, SpawnInfo);
			}
		}
}

bool AFightingGameState::SetMusicStateOnNumOfPlayers_Validate()
{
	return true;
}

void AFightingGameState::SetMusicStateOnNumOfPlayers_Implementation()
{
	UNetworkingGameInstance* GI = Cast<UNetworkingGameInstance>(GetWorld()->GetGameInstance());
	if (GI != nullptr)
	{
		if (LevelMusicManager != nullptr)
		{
			if (GI->WwiseTransitionData.Num() > 0 && GI->WwiseTransitionData.Num() == 2)
			{
				UAkGameplayStatics::SetState("Game_State", "Roof");
				LevelMusicManager->PlayMusicBasedOnStage(0, 5);
			}
			else if (GI->WwiseTransitionData.Num() > 2)
			{
				UAkGameplayStatics::SetState("Game_State", "Street");
				LevelMusicManager->PlayMusicBasedOnStage(0, 1);
			}
		}
	}
}