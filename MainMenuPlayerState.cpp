// Copywrite Shapeshifter 2019

#include "MainMenuPlayerState.h"
#include "MainMenuGameState.h"
#include "Engine.h"

#include "AkComponent.h"
#include "AkGameplayStatics.h"
#include "NetworkingGameInstance.h"

void AMainMenuPlayerState::Trigger_UnpossessPawns()
{
	GS_UnpossessPawns();
}

bool AMainMenuPlayerState::GS_UnpossessPawns_Validate()
{
	return true;
}

void AMainMenuPlayerState::GS_UnpossessPawns_Implementation()
{
	if (Role == ROLE_Authority)
	{
		AMainMenuGameState* GS = GetWorld()->GetGameState<AMainMenuGameState>();
		if (GS != nullptr)
		{
			GS->UnpossessPawns();
		}
	}
}

void AMainMenuPlayerState::BeginPlay()
{
	Super::BeginPlay();

}