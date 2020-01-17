// Copywrite Shapeshifter 2019

#include "MainMenuGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

bool AMainMenuGameState::UnpossessPawns_Validate()
{
	return true;
}

void AMainMenuGameState::UnpossessPawns_Implementation()
{
	for (int i = 0; i < PlayerArray.Num(); i++)
	{
		APlayerController* PC = Cast<APlayerController>(PlayerArray[i]->GetPawn()->GetController());
		if (PC != nullptr)
		{
			PC->UnPossess();
		}
	}
}