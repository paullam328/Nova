// Copywrite Shapeshifter 2019

#include "LevelMusicManager.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"
#include "Kismet/GameplayStatics.h"

#include "NetworkingGameInstance.h"
#include "Engine.h"
#include "AkComponent.h"
#include "FighterController.h"
#include "CharacterSelectController.h"
#include "CharacterPlayerState.h"

// Sets default values
ALevelMusicManager::ALevelMusicManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bReplicates = true;
	bReplicateMovement = true;

}

// Called when the game starts or when spawned
void ALevelMusicManager::BeginPlay()
{
	Super::BeginPlay();
	//UAkGameplayStatics::StopAll();
	//Play: 3 2 1 fight!
	InitializeLevelMusicManager();

}

// Called every frame
void ALevelMusicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ALevelMusicManager::PlayMusicBasedOnStage_Validate(float vol, int stage)
{
	return true;
}

void ALevelMusicManager::PlayMusicBasedOnStage_Implementation(float vol, int stage)
{
	//if (GetNetMode() == ENetMode::NM_Client || GetNetMode() == ENetMode::NM_ListenServer)
	//{
		switch (stage)
		{
		case 1: //only stages 1, 3 and 5 now...
			UAkGameplayStatics::SetState("Game_State", "Street");
			break;

		case 2:
			UAkGameplayStatics::SetState("Game_State", "Club");

			break;

		case 3:
			UAkGameplayStatics::SetState("Game_State", "Club");

			break;

		case 4:
			UAkGameplayStatics::SetState("Game_State", "Club");
			break;

		case 5:
			if (Role == ROLE_Authority)
			{
				UAkGameplayStatics::SetState("Game_State", "Roof");
			}
			break;

		case 10: //Character Select
		{
			UAkGameplayStatics::SetState("Game_State", "CHAR_Select");
			Client_PostMusic(true, false);
		}
		break;

		case 20: //Transition
			UAkGameplayStatics::SetState("Game_State", "Elevator");
			break;

		default:
		{
			UAkGameplayStatics::SetState("Game_State", "Menu");
			Client_PostMusic(true, false);
		}
		break;

		}
}

void ALevelMusicManager::AdjustMusicSlider_Implementation(int stage, bool isFighterInitialization)
{

	UNetworkingGameInstance* NGI = GetGameInstance<UNetworkingGameInstance>();
	AFighterController* FC = Cast<AFighterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (NGI != nullptr)
	{
		UAkGameplayStatics::SetOutputBusVolume(NGI->MusSliderVal, this);

		if (FC != nullptr)
		{
			PlayMusicBasedOnStage(NGI->MusSliderVal, stage);

			if (isFighterInitialization)
			{
				Client_PostMusic(false, true);
			}
		}
		else
		{
			ACharacterSelectController* CSC = Cast<ACharacterSelectController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			
			if (CSC != nullptr)
			{
				PlayMusicBasedOnStage(NGI->MusSliderVal, stage);
			}
		}
	}
}


void ALevelMusicManager::MainMenu_AdjustMusSlider(float MusSliderVal)
{
	UAkGameplayStatics::SetOutputBusVolume(MusSliderVal, this);
}

bool ALevelMusicManager::InitializeLevelMusicManager_Validate()
{
	return true;
}

void ALevelMusicManager::InitializeLevelMusicManager_Implementation()
{
	if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "SessionUITester")
	{
		PlayMusicBasedOnStage(1, 0);
	}
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "CharacterSelection")
	{
		AdjustMusicSlider(10, false);
	}
	else
	{
		AdjustMusicSlider(1, false);
	}
}

bool ALevelMusicManager::Client_PostMusic_Validate(bool IsMainMenu, bool IsStage)
{
	return true;
}

void ALevelMusicManager::Client_PostMusic_Implementation(bool IsMainMenu, bool IsStage)
{
	bool shouldPostMusic = false;
	if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "Stage1")
	{
		//if (GetWorld()->GetNetMode() == NM_ListenServer || GetWorld()->GetNetMode() == NM_Client) //This may solve the bug of music spawning twice?
		//{
		shouldPostMusic = true;
		//}
	}
	else
	{
		shouldPostMusic = true;
	}

	if (shouldPostMusic)
	{
		if (IsMainMenu)
		{
			UAkGameplayStatics::PostEvent(MainMenuEvent, this, false, "");
		}
		else if (IsStage)
		{
			/*if (!isEventPosted && Role < ROLE_Authority)
			{
				SetIsEventPosted();
				UAkGameplayStatics::PostEvent(Stage1Event, this, false, "");
			}
			else if (GetNetMode() == ENetMode::NM_ListenServer)
			{
				UAkGameplayStatics::PostEvent(Stage1Event, this, false, "");
			}*/

			//Try to comment out as below...
			//PostEvent();

			//UAkGameplayStatics::PostEvent(Stage1Event, this, false, "");
		}
	}
}

bool ALevelMusicManager::SetIsEventPosted_Validate()
{
	return true;
}

void ALevelMusicManager::SetIsEventPosted_Implementation()
{
	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		isEventPosted = true;
	}
}

void ALevelMusicManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALevelMusicManager, isEventPosted);

}

void ALevelMusicManager::PostEvent()
{
	UAkGameplayStatics::PostEvent(Stage1Event, this, false, "");
}