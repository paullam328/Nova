// Copywrite Shapeshifter 2019

#include "FighterPlayerState.h"
#include "FighterController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "CharacterSelectGameManager.h"

#include "FighterBase.h"

#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"
#include "NetworkingGameInstance.h"
#include "WwiseTransitionData.h"
#include "AkComponent.h"

void AFighterPlayerState::PostInitializeComponents()
{

	Super::PostInitializeComponents();

}

void AFighterPlayerState::UpdatePing(float InPing)
{
	// Nothing here
}

void AFighterPlayerState::CalculatePing(float NewPing)
{
	// Ignoring overflow
	if (NewPing < 0.f)
	{
		return;
	}

	float OldPing = ExactPing;
	Super::UpdatePing(NewPing);

	AFighterController* PC = Cast<AFighterController>(GetOwner());

	if (!PC)
	{
		return;
	}

	PC->LastPingUpdateTime = GetWorld()->GetTimeSeconds();

	if (ExactPing != OldPing) {
		PC->ServerUpdatePing(ExactPing);
	}
}

// Called when the game starts or when spawned
void AFighterPlayerState::BeginPlay()
{
	SetPlayersNames();
}

bool AFighterPlayerState::SetPlayersNames_Validate()
{
	return true;
}

void AFighterPlayerState::SetPlayersNames_Implementation()
{
	if (Role == ROLE_Authority)
	{
		nameIndex = FMath::RandRange(0, Names.Num() - 1);
		this->SetPlayerName(Names[nameIndex]);
	}
}


void AFighterPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFighterPlayerState, isSelectedCharSet);
	DOREPLIFETIME(AFighterPlayerState, PlayerIndex);
	DOREPLIFETIME(AFighterPlayerState, PawnToPossess);
	DOREPLIFETIME(AFighterPlayerState, IsEventPosted);
	DOREPLIFETIME(AFighterPlayerState, nameIndex);
}

bool AFighterPlayerState::Trigger_InstantiateStageSwitchWipeWidget_Validate()
{
	return true;
}

void AFighterPlayerState::Trigger_InstantiateStageSwitchWipeWidget_Implementation()
{
	if (Role == ROLE_Authority)
	{
		AFighterController* PC = Cast<AFighterController>(GetOwner()); 
		if (PC != nullptr)
		{
			//PC->InstantiateStageSwitchWipeWidget();
		}
	}
}


bool AFighterPlayerState::AdjustVolumeSlider_Validate()
{
	return true;
}

void AFighterPlayerState::AdjustVolumeSlider_Implementation()
{
	float val = 1;
	for (int i = 0; i < GetGameInstance<UNetworkingGameInstance>()->WwiseTransitionData.Num(); i++)
	{
		FWwiseTransitionData data = GetGameInstance<UNetworkingGameInstance>()->WwiseTransitionData[i];
		if (data.GetPlayerName() == GetPlayerName())
		{
			val = data.GetMusSliderVal();
			FString str = "Mus_Slider";
			UAkGameplayStatics::SetRTPCValue(FName(*str), val, 0, GetPawn()->GetController());
		}
	}
}