// Copywrite Shapeshifter 2019

#include "MainMenuController.h"
#include "NetworkingGameInstance.h"
#include "LevelMusicManager.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	bEnableMouseOverEvents = false;
	bEnableMouseOverEvents = false;
}

void AMainMenuController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AMainMenuController, MusSliderVal);
	DOREPLIFETIME(AMainMenuController, SFXSliderVal);
	DOREPLIFETIME(AMainMenuController, VoiceSliderVal);
}

void AMainMenuController::SetMusSliderVal(float Val)
{
	MusSliderVal = Val;
}

void AMainMenuController::SetSFXSliderVal(float Val)
{
	SFXSliderVal = Val;
}

void AMainMenuController::SetVoiceSliderVal(float Val)
{
	VoiceSliderVal = Val;
}

void AMainMenuController::StoreWwiseDataToGI()
{
	GetGameInstance<UNetworkingGameInstance>()->StoreWwiseTransitionData(MusSliderVal, SFXSliderVal, VoiceSliderVal);
}