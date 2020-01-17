// Copywrite Shapeshifter 2019

#include "CharacterSelectionGameMode.h"
#include "CharacterSelectWidget.h"
#include "CharacterSelectGameState.h"
#include "NetworkingGameInstance.h"
#include "CharacterSelectController.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Runtime/EngineSettings/Classes/GameMapsSettings.h"
#include "Engine.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "CharacterSelectTimerState.h"
#include "CharacterPlayerState.h"


ACharacterSelectionGameMode::ACharacterSelectionGameMode()
{
	bUseSeamlessTravel = true;
	bActorSeamlessTraveled = false;

	
}


void ACharacterSelectionGameMode::RegisterServerSteam() {

	if (GetNetMode() == NM_DedicatedServer)
	{
		IOnlineSubsystem* OSub = IOnlineSubsystem::Get(FName("Steam"));
		bool bLoginStatus = OSub->GetIdentityInterface()->AutoLogin(0);
		if (!OSub || !bLoginStatus)
		{
			UE_LOG(LogOnline, Warning, TEXT("Steam authentication failed"));
		}
		else
		{
			UE_LOG(LogOnline, Warning, TEXT("Authentication succeful"));
		}

	}
}



void ACharacterSelectionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	
	
	//SetPlayerIndex(NewPlayer);
	PlayerControllerList.Add(NewPlayer);

	ACharacterSelectGameState* CSGS = GetGameState<ACharacterSelectGameState>();
	if (CSGS != nullptr)
	{
		CSGS->timerState = ECharacterSelectTimerState::WaitingForTwoPlayers;
		if (CSGS->ArePlayersReady.IsValidIndex(PlayerControllerList.Num() - 1))
		{
			CSGS->ArePlayersReady[PlayerControllerList.Num() - 1] = EPlayerReady::NotReady;
		}

		//If the gamestate timerstate is maintimer/banner, reset the timer and banner:
		if (CSGS->timerState == ECharacterSelectTimerState::MainCountdown
			|| CSGS->timerState == ECharacterSelectTimerState::GetReady)
		{
			CSGS->SetBannerNotReadyAllPlayers();
		}

	}

}

bool ACharacterSelectionGameMode::Server_ServerTravel_Validate()
{
	return true;
}

void ACharacterSelectionGameMode::Server_ServerTravel_Implementation()
{

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (IsEditorOnly())
	{
		bUseSeamlessTravel = false;
	}

	ProcessServerTravel("/Game/Maps/Stage1?listen", true);
}


void ACharacterSelectionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	APlayerController* PC = Cast<APlayerController>(Exiting);

	int setNonExistentIndex = 0;
	for (int i = 0; i < PlayerControllerList.Num(); i++)
	{
		if (PlayerControllerList[i] == PC)
		{
			setNonExistentIndex = i;
			PlayerControllerList.Remove(PC);
		}
	}

	if (PlayerControllerList.Num() <= 2)
	{
		//Set back to two players state
		ACharacterSelectGameState* CSGS = GetGameState<ACharacterSelectGameState>();
		if (CSGS != nullptr)
		{
			CSGS->timerState = ECharacterSelectTimerState::WaitingForTwoPlayers;
			if (CSGS->ArePlayersReady.IsValidIndex(setNonExistentIndex))
			{
				CSGS->ArePlayersReady[setNonExistentIndex] = EPlayerReady::NonExistent;
			}
		}
	}

}