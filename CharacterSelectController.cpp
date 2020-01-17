// Copywrite Shapeshifter 2019

#include "CharacterSelectController.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "Engine.h"
#include "EngineSettings/Classes/GameMapsSettings.h"
#include "CharacterSelectWidget.h"
#include "CharacterSelectGameState.h"
#include "StageSwitchWipeWidget.h"

#include "AkAudioEvent.h"
#include "AkComponent.h"
#include "AkGameplayStatics.h"
#include "NetworkingGameInstance.h"
#include "WwiseTransitionData.h"
#include "MaskiumCharacterSelectActor.h"
#include "DemarkCharacterSelectActor.h"

#include "CharacterPlayerState.h"


ACharacterSelectController::ACharacterSelectController()
{
	bReplicates = true;

	bShowMouseCursor = false;
	bEnableMouseOverEvents = false;
	bEnableMouseOverEvents = false;
}


void ACharacterSelectController::HoldBToLeave()
{
	
	if (CharacterSelectWidget != nullptr)
	{
		CharacterSelectWidget->HoldToLeave();
	}
}

void ACharacterSelectController::ReleaseB()
{
	if (CharacterSelectWidget != nullptr)
	{
		CharacterSelectWidget->ReleaseLeave();
	}
}

void ACharacterSelectController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);

	//GetWorld()->ServerTravel("/Game/Maps/Stage1?listen", true, true);
}

bool ACharacterSelectController::Trigger_ServerTravel_Validate()
{
	return true;
}

void ACharacterSelectController::Trigger_ServerTravel_Implementation()
{

	if (Role == ROLE_Authority)
	{

		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		World->ServerTravel("/Game/Maps/Stage1?listen", true, false);
	}

}

void ACharacterSelectController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}
	UWorld* World = GetWorld();
	if (World)
	{

		if(IsLocalController())
		{
		
		
			if (CharacterSelectWidget == nullptr && CharacterSelectWidgetClass != nullptr)
			{
				FillArray();

				CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(World, CharacterSelectWidgetClass);
				CharacterSelectWidget->AddToViewport();

				//UGameplayStatics::GetPlayerController(World, 0)->SetInputMode();
				//APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

				UWidgetBlueprintLibrary::SetInputMode_GameAndUI(this, CharacterSelectWidget->Character1Button, false);

				TArray<AActor*> arrOfCams;
				UGameplayStatics::GetAllActorsOfClass(World, ACameraActor::StaticClass(), arrOfCams);
				ACameraActor* cam = Cast<ACameraActor>(arrOfCams[0]);
				this->SetViewTargetWithBlend(cam, 0.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);

				ChangeSelectImg();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Widget is detached from CharacterSelectGameManager_BP.  Please re-attach them!")));
			}
		}
	}
	SetAkComp();
	AdjustVoiceSlider();
}

void ACharacterSelectController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Special", IE_Repeat, this, &ACharacterSelectController::HoldBToLeave);
	InputComponent->BindAction("Special", IE_Released, this, &ACharacterSelectController::ReleaseB);
	InputComponent->BindAction("Mouse1", IE_Pressed, this, &ACharacterSelectController::MissClick);
	InputComponent->BindAction("Mouse2", IE_Pressed , this, &ACharacterSelectController::MissClick);
}

bool ACharacterSelectController::SetAkComp_Validate()
{
	return true;
}

void ACharacterSelectController::SetAkComp_Implementation()
{
		if (GetGameInstance<UNetworkingGameInstance>()->AkComp != nullptr)
		{
			AkComp = GetGameInstance<UNetworkingGameInstance>()->AkComp;
		}
}

// Called every frame
void ACharacterSelectController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

		if (CharacterSelectWidget != nullptr)
		{
			SetNum(CharacterSelectWidget->CharNum);

			/*for (int i = 0; i < allModels.Num(); i++)
			{
				if (i == num)
				{
					allModels[i]->GetSkeletalMeshComponent()->SetVisibility(true);
				}
				else
				{
					allModels[i]->GetSkeletalMeshComponent()->SetVisibility(false);
				}
			}*/

			DetectClickStart();
			DetectBannerClickBack();
			SetMyPlayerName();

		}

		//Uncomment:
		if (StageSwitchWipeWidget != nullptr)
		{
			//StageSwitchWipeWidget->Wipe(true);
			StageSwitchWipeWidget->isOnTickWipeActivated = true;
		}
}

void ACharacterSelectController::FillArray()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString idle = "Idle";
		TArray<AActor*> idles;
		UGameplayStatics::GetAllActorsWithTag(World, FName(*idle), idles);

		FString maskium = "Maskium";
		TArray<AActor*> masks;
		UGameplayStatics::GetAllActorsWithTag(World, FName(*maskium), masks);

		allModels.Add(Cast<ASkeletalMeshActor>(idles[0]));
		allModels.Add(Cast<ASkeletalMeshActor>(masks[0]));
	}
}

void ACharacterSelectController::MissClick()
{
	FString widgetName = CharacterSelectWidget->FocusedWidget->GetName();
	TSharedPtr<SWidget> widgetToFocus = CharacterSelectWidget->GetSlateWidgetFromName(FName(*widgetName));
	SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(widgetToFocus));
}



// Called to bind functionality to input
//void ACharacterSelectController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}



void ACharacterSelectController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterSelectController, charEnum);
	DOREPLIFETIME(ACharacterSelectController, PlayerIndex);
	DOREPLIFETIME(ACharacterSelectController, num);
	DOREPLIFETIME(ACharacterSelectController, arrOfSelectedCharIndices);
	DOREPLIFETIME(ACharacterSelectController, IsIndexSet);
	DOREPLIFETIME(ACharacterSelectController, arrOfPlayersReady);
	DOREPLIFETIME(ACharacterSelectController, PlayerReadyState);
}

bool ACharacterSelectController::SetCharEnumSelected_Validate()
{
	return true;
}

void ACharacterSelectController::SetCharEnumSelected_Implementation()
{
	if (Role == ROLE_Authority)
	{
		switch (num + 1)
		{
		case 1:
			charEnum = ECharacter::Demark;
			break;
		case 2:
			charEnum = ECharacter::Maskium;
			break;
		default:
			break;
		}
	}
}


bool ACharacterSelectController::SetMyPlayerName_Validate()
{
	return true;
}

void ACharacterSelectController::SetMyPlayerName_Implementation()
{
	//if (Role == ROLE_Authority && IsLocallyControlled())
	//{ 
	/*if (PlayerIndex > -1 && CharacterSelectWidget != nullptr)
	{
		if (CharacterSelectWidget->PlayerNames.IsValidIndex(0)
			&& CharacterSelectWidget->PlayerNames.Num() == 6)
		{
			CharacterSelectWidget->PlayerNames[PlayerIndex]->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)));
		}
	}*/

	if (CharacterSelectWidget != nullptr)
	{
		ACharacterPlayerState* CPS = GetPlayerState<ACharacterPlayerState>();
		if (CPS != nullptr)
		{
			CharacterSelectWidget->SetSelfNameGreen(CPS->GetPlayerIndex());
		}
	}

	//}
}

bool ACharacterSelectController::SetNum_Validate(int CharNum)
{
	return true;
}

void ACharacterSelectController::SetNum_Implementation(int CharNum)
{
	if (Role == ROLE_Authority)
	{
		num = CharNum;
	}
}


bool ACharacterSelectController::UpdateSelectArray_Validate()
{
	return true;
}

void ACharacterSelectController::UpdateSelectArray_Implementation()
{
	if (Role == ROLE_Authority)
	{
		ACharacterSelectGameState* GS = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());

		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			UPlayer* Player = PlayerState->GetNetOwningPlayer();
			APlayerController* Controller = Player->GetPlayerController(World);
			
		}

		if (GS != nullptr)
		{
			if (!arrOfSelectedCharIndices.IsValidIndex(0) || arrOfSelectedCharIndices.Num() != 6)
			{
				//Initialize the array if not
				arrOfSelectedCharIndices.Add(-1);
				arrOfSelectedCharIndices.Add(-1);
				arrOfSelectedCharIndices.Add(-1);
				arrOfSelectedCharIndices.Add(-1);
				arrOfSelectedCharIndices.Add(-1);
				arrOfSelectedCharIndices.Add(-1);
			}

			if (GS->SelectedCharIndices.Num() == 6
				&& GS->SelectedCharIndices.IsValidIndex(0))
			{
				if (arrOfSelectedCharIndices[0] != GS->SelectedCharIndices[0]
					|| arrOfSelectedCharIndices[1] != GS->SelectedCharIndices[1]
					|| arrOfSelectedCharIndices[2] != GS->SelectedCharIndices[2]
					|| arrOfSelectedCharIndices[3] != GS->SelectedCharIndices[3]
					|| arrOfSelectedCharIndices[4] != GS->SelectedCharIndices[4]
					|| arrOfSelectedCharIndices[5] != GS->SelectedCharIndices[5])
				{
					for (int j = 0; j < arrOfSelectedCharIndices.Num(); j++)
					{
						arrOfSelectedCharIndices[j] = GS->SelectedCharIndices[j];
					}
				}
			}
		}
	}
}

bool ACharacterSelectController::UpdatePlayersReady_Validate()
{
	return true;
}

void ACharacterSelectController::UpdatePlayersReady_Implementation()
{
	if (Role == ROLE_Authority)
	{
		ACharacterSelectGameState* GS = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());
		if (GS != nullptr)
		{
			if (!arrOfPlayersReady.IsValidIndex(0)
				|| arrOfPlayersReady.Num() != 6)
			{
				//Initialize the array if not
				arrOfPlayersReady.Add(EPlayerReady::NonExistent);
				arrOfPlayersReady.Add(EPlayerReady::NonExistent);
				arrOfPlayersReady.Add(EPlayerReady::NonExistent);
				arrOfPlayersReady.Add(EPlayerReady::NonExistent);
				arrOfPlayersReady.Add(EPlayerReady::NonExistent);
				arrOfPlayersReady.Add(EPlayerReady::NonExistent);
			}

			if (GS->ArePlayersReady.Num() == 6
				&& GS->ArePlayersReady.IsValidIndex(0))
			{
				if (arrOfPlayersReady[0] != GS->ArePlayersReady[0]
					|| arrOfPlayersReady[1] != GS->ArePlayersReady[1]
					|| arrOfPlayersReady[2] != GS->ArePlayersReady[2]
					|| arrOfPlayersReady[3] != GS->ArePlayersReady[3]
					|| arrOfPlayersReady[4] != GS->ArePlayersReady[4]
					|| arrOfPlayersReady[5] != GS->ArePlayersReady[5])
				{
					for (int j = 0; j < arrOfPlayersReady.Num(); j++)
					{
						arrOfPlayersReady[j] = GS->ArePlayersReady[j];
					}
				}
			}
		}
	}
}

bool ACharacterSelectController::ChangeSelectImg_Validate()
{
	return true;
}

void ACharacterSelectController::ChangeSelectImg_Implementation()
{
	if (IsLocalController())
	{
		if (Role == ROLE_Authority)
		{
			SetCharEnumSelected();
			Cast<ACharacterSelectGameState>(GetWorld()->GetGameState())->SetSelectedCharPerPlayer(PlayerIndex, charEnum);
			UpdateSelectArray();
			TriggerSetCharacterImages();
		}
	}
}

void ACharacterSelectController::Trigger_ChangeSelectImg()
{
	ChangeSelectImg();
}

bool ACharacterSelectController::ChangePlayerReady_Validate()
{
	return true;
}

void ACharacterSelectController::ChangePlayerReady_Implementation()
{
	if (Role == ROLE_Authority)
	{

		Cast<ACharacterSelectGameState>(GetWorld()->GetGameState())->SetPlayerReadyPerPlayer(PlayerIndex);
		//Cast<ACharacterSelectGameState>(GetWorld()->GetGameState())->
	}
}

void ACharacterSelectController::Trigger_PlayerReady()
{
	ChangePlayerReady();
}

bool ACharacterSelectController::SetPlayerNotReady_Validate()
{
	return true;
}

void ACharacterSelectController::SetPlayerNotReady_Implementation()
{
	if (Role == ROLE_Authority)
	{
		Cast<ACharacterSelectGameState>(GetWorld()->GetGameState())->SetPlayerNotReadyPerPlayer(PlayerIndex);
	}
}

bool ACharacterSelectController::TravelPlayer_Validate()
{
	return true;
}

void ACharacterSelectController::TravelPlayer_Implementation()
{
	if (Role == ROLE_Authority)
	{
		FString nMapName = "Stage1";
		UGameplayStatics::OpenLevel(GetWorld(), FName(*nMapName), true, "listen");
	}
}


bool ACharacterSelectController::TriggerSetCharacterImages_Validate()
{
	return true;
}

void ACharacterSelectController::TriggerSetCharacterImages_Implementation()
{
	if (IsLocalController())
	{
		CharacterSelectWidget->SetCharacterImgs();
	}

}

bool ACharacterSelectController::InstantiateStageSwitchWipeWidget_Validate()
{
	return true;
}

void ACharacterSelectController::InstantiateStageSwitchWipeWidget_Implementation()
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	UWorld* World = GetWorld();
	// Have to check whether it's local controller to spawn the widget in client-side!!!
	// or else it would spawn in the server controller instead and not the client:
	if ((IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer) && StageSwitchWipeWidget == nullptr)
	{
		StageSwitchWipeWidget = CreateWidget<UStageSwitchWipeWidget>(World, StageSwitchWipeWidgetClass);
		//uncomment:
		StageSwitchWipeWidget->AddToViewport();

	}
}

bool ACharacterSelectController::Trigger_PlayAlertAnim_Validate()
{
	return true;
}

void ACharacterSelectController::Trigger_PlayAlertAnim_Implementation()
{
	if (CharacterSelectWidget != nullptr)
	{
		//this mightve triggered the banner..
		//CharacterSelectWidget->PlayAnim();
	}
}


bool ACharacterSelectController::Server_PlayAlertAnim_Validate()
{
	return true;
}

void ACharacterSelectController::Server_PlayAlertAnim_Implementation()
{
	if (Role == ROLE_Authority)
	{
		Trigger_PlayAlertAnim();
	}
}

bool ACharacterSelectController::RemoveCharacterSelectionWidget_Validate()
{
	return true;
}

void ACharacterSelectController::RemoveCharacterSelectionWidget_Implementation()
{
	if (CharacterSelectWidget != nullptr)
	{

		CharacterSelectWidget->Widget_RemoveFromParent();
		CharacterSelectWidget = nullptr;
	}

}

void ACharacterSelectController::AdjustVoiceSlider()
{
	UNetworkingGameInstance* NGI = GetGameInstance<UNetworkingGameInstance>();
	if (NGI != nullptr)
	{
		UAkGameplayStatics::SetOutputBusVolume(NGI->VoiceSliderVal,this);
	}
}


void ACharacterSelectController::Trigger_GetPlayerName()
{
	Server_GetPlayerName();
}

bool ACharacterSelectController::Server_GetPlayerName_Validate()
{
	return true;
}

void ACharacterSelectController::Server_GetPlayerName_Implementation()
{
	ACharacterPlayerState* CPS = GetPlayerState<ACharacterPlayerState>();
	if (CPS != nullptr)
	{
		UI_PlayerName = CPS->GetPlayerName();
	}
}

bool ACharacterSelectController::PlayTransitionSound_Validate()
{
	return true;
}

void ACharacterSelectController::PlayTransitionSound_Implementation()
{
	UAkGameplayStatics::PostEvent(TransitionEvent, this, false, "");
}

bool ACharacterSelectController::Server_PopupBanner_Validate()
{
	return true;
}

void ACharacterSelectController::Server_PopupBanner_Implementation()
{
	if (CharacterSelectWidget != nullptr)
	{
		CharacterSelectWidget->ReadyPopupBanner();
	}
}

bool ACharacterSelectController::SetBannerReady_Validate()
{
	return true;
}

void ACharacterSelectController::SetBannerReady_Implementation()
{
	ACharacterPlayerState* CPS = GetPlayerState<ACharacterPlayerState>();

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		CPS->SetBannerReady();
	}
}

bool ACharacterSelectController::DetectClickStart_Validate()
{
	return true;
}

void ACharacterSelectController::DetectClickStart_Implementation()
{
	//Set input focus to both game (input detection) and UI (Widget focus)
	
	if (CharacterSelectWidget->IsBannerReady)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUI(this, CharacterSelectWidget, false, true);


		if (WasInputKeyJustPressed(EKeys::Gamepad_Special_Right)
			|| WasInputKeyJustPressed(EKeys::A))
		{
			CharacterSelectWidget->SetReadyBannerColor_Ready();
		}
	}
}

bool ACharacterSelectController::DetectBannerClickBack_Validate()
{
	return true;
}

void ACharacterSelectController::DetectBannerClickBack_Implementation()
{
	//Set input focus to both game (input detection) and UI (Widget focus)

	if (CharacterSelectWidget->IsBannerReady)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUI(this, CharacterSelectWidget, false, true);


		if (WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right))
		{
			Server_SetNotBannerReady();
		}
	}
}

bool ACharacterSelectController::Client_HideBanner_Validate()
{
	return true;
}

void ACharacterSelectController::Client_HideBanner_Implementation()
{
	CharacterSelectWidget->HideBanner();
	CharacterSelectWidget->StartButton_SetColor(false);
	CharacterSelectWidget->Widget_SetUserFocus("One", this);

	CharacterSelectWidget->LerpAlpha = 0;
	CharacterSelectWidget->LerpStart = 0;
	CharacterSelectWidget->LerpTarget = 1;
}

bool ACharacterSelectController::Server_SetNotBannerReady_Validate()
{
	return true;
}

void ACharacterSelectController::Server_SetNotBannerReady_Implementation()
{
	ACharacterSelectGameState* GS = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());

	if (GS != nullptr)
	{
		GS->SetBannerNotReadyAllPlayers();
	}
}

bool ACharacterSelectController::Server_GetTimer_Validate()
{
	return true;
}

void ACharacterSelectController::Server_GetTimer_Implementation()
{
	ACharacterSelectGameState* GS = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());

	if (GS != nullptr)
	{
		Client_GetTimer(GS->timer);
	}
}
bool ACharacterSelectController::Client_GetTimer_Validate(float timerSec)
{
	return true;
}

void ACharacterSelectController::Client_GetTimer_Implementation(float timerSec)
{
	CharacterSelectWidget->Widget_timerSec = timerSec;
}

bool ACharacterSelectController::Server_GetShouldShowtimer_Validate()
{
	return true;
}

void ACharacterSelectController::Server_GetShouldShowtimer_Implementation()
{
	ACharacterSelectGameState* GS = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());

	if (GS != nullptr)
	{
		if (GS->timerState == ECharacterSelectTimerState::MainCountdown)
		{
			Client_GetShouldShowTimer(true);
		}
		else
		{
			Client_GetShouldShowTimer(false);
		}
	}
}

bool ACharacterSelectController::Client_GetShouldShowTimer_Validate(bool ShouldShowTimer)
{
	return true;
}

void ACharacterSelectController::Client_GetShouldShowTimer_Implementation(bool ShouldShowTimer)
{
	CharacterSelectWidget->ShouldShowTimer = ShouldShowTimer;
}

void ACharacterSelectController::SelectMaskium()
{
	TArray<AActor*> actor;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Maskium")), actor);

	if (actor[0] != nullptr)
	{

		AMaskiumCharacterSelectActor* mesh = Cast<AMaskiumCharacterSelectActor>(actor[0]);
		if (mesh != nullptr)
		{
			mesh->SetSelected(true);

			TArray<AActor*> actor1;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Idle")), actor1);

			if (actor1[0] != nullptr)
			{

				ADemarkCharacterSelectActor* mesh = Cast<ADemarkCharacterSelectActor>(actor1[0]);
				if (mesh != nullptr)
				{
					mesh->SetSelected(false);
				}
			}
		}
	}
	
}

void ACharacterSelectController::SelectDemark()
{
	TArray<AActor*> actor;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Idle")), actor);

	if (actor[0] != nullptr)
	{
		ADemarkCharacterSelectActor* mesh = Cast<ADemarkCharacterSelectActor>(actor[0]);
		if (mesh != nullptr)
		{
			mesh->SetSelected(true);

			TArray<AActor*> actor1;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Maskium")), actor1);

			if (actor1[0] != nullptr)
			{

				AMaskiumCharacterSelectActor* mesh = Cast<AMaskiumCharacterSelectActor>(actor1[0]);
				if (mesh != nullptr)
				{
					mesh->SetSelected(false);
				}
			}
		}
	}
}
