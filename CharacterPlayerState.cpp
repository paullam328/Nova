// Copywrite Shapeshifter 2019

#include "CharacterPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterSelectGameState.h"
#include "Engine.h"
#include "NetworkingGameInstance.h"
#include "AkGameplayStatics.h"

/*bool ACharacterPlayerState::ChangeSelectImg_Validate()
{
	return true;
}
void ACharacterPlayerState::ChangeSelectImg_Implementation()
{
	UpdateSelectArray();
}*/


void ACharacterPlayerState::BeginPlay()
{
	Super::BeginPlay();

	SetSFXVolume();
}

void ACharacterPlayerState::SetSFXVolume()
{
	UNetworkingGameInstance* NGI = GetGameInstance<UNetworkingGameInstance>();
	if (NGI != nullptr)
	{
		UAkGameplayStatics::SetOutputBusVolume(NGI->SFXSliderVal, this);
	}
	
}

void ACharacterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterPlayerState, PlayerIndex);
	DOREPLIFETIME(ACharacterPlayerState, charEnum);
	DOREPLIFETIME(ACharacterPlayerState, NumOfSelected);
	DOREPLIFETIME(ACharacterPlayerState, IsPlayerReady);
}

void ACharacterPlayerState::Trigger_SelectCharacter(int32 cNum)
{
	Server_SetAndGetSelected(cNum);
	
}


bool ACharacterPlayerState::Server_SetAndGetSelected_Validate(int32 cNum)
{
	return true;
}
void ACharacterPlayerState::Server_SetAndGetSelected_Implementation(int32 cNum)
{

	if (Role == ROLE_Authority)
	{
		ACharacterSelectGameState* CSGS = Cast<ACharacterSelectGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (CSGS != nullptr)
		{
			//Set in the game state
			CSGS->InsertSelectedCharIntoArray(PlayerIndex, cNum);

			//Then get from the game state to ensure "Handshake"
			//CSGS->GetSelectedCharFromArray(PlayerIndex, NumOfSelected);
			//}

			if (CSGS->SelectedCharIndices.IsValidIndex(PlayerIndex))
			{
				NumOfSelected = CSGS->SelectedCharIndices[PlayerIndex];
			}
			else
			{
				FString invalid = "invalid";
				UE_LOG(LogTemp, Warning, TEXT("Server_SetAndGetSelected Invalid"));
			}
		}

	}
}

void ACharacterPlayerState::Trigger_SetPlayerReady(EPlayerReady IsReady)
{
	Server_SetAndGetPlayerReady(IsReady);

}

bool ACharacterPlayerState::Server_SetAndGetPlayerReady_Validate(EPlayerReady IsReady)
{
	return true;
}
void ACharacterPlayerState::Server_SetAndGetPlayerReady_Implementation(EPlayerReady IsReady)
{

	if (Role == ROLE_Authority)
	{
		ACharacterSelectGameState* CSGS = Cast<ACharacterSelectGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (CSGS != nullptr)
		{
			//Set in the game state
			CSGS->SetPlayerReadyInArray(PlayerIndex, IsReady);

			//Then get from the game state to ensure "Handshake"
			//CSGS->GetSelectedCharFromArray(PlayerIndex, NumOfSelected);
			//}

			if (CSGS->ArePlayersReady.IsValidIndex(PlayerIndex))
			{
				IsPlayerReady = CSGS->ArePlayersReady[PlayerIndex];
			}
			else
			{
				FString invalid = "invalid";
				UE_LOG(LogTemp, Warning, TEXT("Server_SetAndGetPlayerReady Invalid"));
			}
		}

	}
}

bool ACharacterPlayerState::Trigger_CheckReadyServerTravel_Validate()
{
	return true;
}

void ACharacterPlayerState::Trigger_CheckReadyServerTravel_Implementation()
{
	ACharacterSelectGameState* CSGS = Cast<ACharacterSelectGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (CSGS != nullptr)
	{
		CSGS->Server_CheckReadyServerTravel();
	}
}


bool ACharacterPlayerState::SetBannerReady_Validate()
{
	return true;
}

void ACharacterPlayerState::SetBannerReady_Implementation()
{
	if (Role == ROLE_Authority)
	{
		ACharacterSelectGameState* CSGS = Cast<ACharacterSelectGameState>(UGameplayStatics::GetGameState(GetWorld()));

		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			CSGS->SetBannerReadyPerPlayer(PlayerIndex);
		}
	}

}