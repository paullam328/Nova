// Copywrite Shapeshifter 2019

#include "FighterController.h"
#include "FighterPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "StageSwitchWipeWidget.h"
#include "Textblock.h"
#include "MiniLeaderboardWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "FighterBase.h"
#include "TimerScreen.h"
#include "gamePlayUserHUD.h"
#include "gamePlayUserWidget.h"
#include "PlayerIconWidget.h"
#include "PanelWidget.h"
#include "NetworkingGameInstance.h"
#include "AkComponent.h"
#include "EliminationWidget.h"
#include "StageChangeWidget.h"


// Sets default values
AFighterController::AFighterController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = false;
	bEnableMouseOverEvents = false;
	bEnableMouseOverEvents = false;

	bReplicates = true;
	bReplicateMovement = true;

}

void AFighterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Pause", IE_Pressed, this, &AFighterController::Pause);
}

void AFighterController::Pause() 
{
	if (HUD != nullptr)
	{
		UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
		UGameplayStatics::GetPlayerController(this, 0)->bEnableClickEvents = false;
		UGameplayStatics::GetPlayerController(this, 0)->bEnableMouseOverEvents = false;
		HUD->Pause();
	}
}



void AFighterController::CheckSendPing()
{
	if (HasAuthority()) {
		// We should only send ping updates from server 
		return;
	}
	
	const float WorldSeconds = GetWorld()->GetTimeSeconds();

	if ((WorldSeconds - LastPingUpdateTime) > PingUpdateInteral)
	{
		LastPingUpdateTime = WorldSeconds;
		ServerBouncePing(WorldSeconds);
	}
}

float AFighterController::GetPredictionTime()
{
	if (!PlayerState)
	{
		return 0.f;
	}

	const float ClampedPingMillis = FMath::Clamp(PlayerState->ExactPing, 0.f, MaxPredictionPing);
	const float ClampedPingSeconds = (ClampedPingMillis / 1000.0f);

	return ClampedPingSeconds * PredicitionCorrectionFactor;
}

void AFighterController::PlayerTick(float DeltaTime)
{


	Super::PlayerTick(DeltaTime);

	Multicast_GetPlayerName();
	MiniLeaderboard_GetPlayerName();
	
	// Checking the Send Ping in Tick 
	CheckSendPing();

	SetHasWiped();
	
	if (IsDevMode)
	{
		SetTransitionScreenTransparent();
	}
	UpdatePlayerHealth();

	SetEliminationWidgetNullptr();
}

void AFighterController::InitPlayerState()
{
	Super::InitPlayerState();

	MPlayerState = Cast<AFighterPlayerState>(PlayerState);

	// Setting a default player name for the player...
	if (PlayerState && PlayerState->GetPlayerName().IsEmpty())
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			PlayerState->SetPlayerName("Player");
		}
	}
}

void AFighterController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	MPlayerState = Cast<AFighterPlayerState>(PlayerState);
}

bool AFighterController::ServerUpdatePing_Validate(float TimeStamp) {
	return true;
}

void AFighterController::ServerUpdatePing_Implementation(float ExactPing) {

	if (!MPlayerState) {
		return;
	}

	MPlayerState->ExactPing = ExactPing;

	// We need to compress ExactPing (float) to Ping (int32) for replication - see docs for PlayerState:
	// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/GameFramework/APlayerState/index.html
	MPlayerState->Ping = (int32)(ExactPing / 4);

	// Handle overflow
	if (MPlayerState->Ping < 0) {
		MPlayerState->Ping = 255;
		MPlayerState->ExactPing = (float)(MPlayerState->Ping * 4);
	}

}

bool AFighterController::ServerBouncePing_Validate(float TimeStamp) {
	return true;
}

void AFighterController::ServerBouncePing_Implementation(float TimeStamp) {
	ClientReturnPing(TimeStamp);
}

void AFighterController::ClientReturnPing_Implementation(float TimeStamp) {

	if (!MPlayerState) {
		return;
	}

	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeStamp;
	MPlayerState->CalculatePing(RoundTripTime);
}

bool AFighterController::InstantiateStageSwitchWipeWidget_Validate(bool isWipeAppear, bool isFirstWipe)
{
	return true;
}

void AFighterController::InstantiateStageSwitchWipeWidget_Implementation(bool isWipeAppear, bool isFirstWipe)
{
	//if (IsLocalController())
	//{
		UWorld* World = GetWorld();
		if ((IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer) && StageSwitchWipeWidget == nullptr)
		{
			//need to remove widgets before wiping, due to seamless travel, while not timer/else
			if (isFirstWipe)
			{
				Initialize_RemoveAllWidgets();
				InstantiateTimerScreen();
			}

			StageSwitchWipeWidget = CreateWidget<UStageSwitchWipeWidget>(World, StageSwitchWipeWidgetClass);
			StageSwitchWipeWidget->AddToViewport();

			//StageSwitchWipeWidget->GenerateFactText();

			if (!isWipeAppear)
			{
				StageSwitchWipeWidget->SetDisolve(2.0f);
				//StageSwitchWipeWidget->FactText->SetVisibility(ESlateVisibility::Visible);
				//StageSwitchWipeWidget->Text->SetVisibility(ESlateVisibility::Visible);
				StageSwitchWipeWidget->isTextInitialized = true;
			}
			else
			{
				StageSwitchWipeWidget->SetDisolve(-0.1f);
				//See whether below works...
				StageSwitchWipeWidget->isOnTickWipeActivated = true;
			}
		}
	//}
}

bool AFighterController::Server_InstantiateStageSwitchWipeWidget_Validate(bool isWipeAppear)
{
	return true;
}

void AFighterController::Server_InstantiateStageSwitchWipeWidget_Implementation(bool isWipeAppear)
{
	if (Role == ROLE_Authority)
	{
		InstantiateStageSwitchWipeWidget(isWipeAppear, false);
	}
}

bool AFighterController::WipeTransitionScreen_Validate(bool isWipeAppear)
{
	return true;
}


void AFighterController::WipeTransitionScreen_Implementation(bool isWipeAppear)
{
	if (IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
	{
		if (StageSwitchWipeWidget != nullptr)
		{
			StageSwitchWipeWidget->Wipe(isWipeAppear);
		}
	}
}

bool AFighterController::SpawnGameplayWidgets_Validate()
{
	return true;
}

void AFighterController::SpawnGameplayWidgets_Implementation()
{
	


	/*HUD = Cast<AgamePlayUserHUD>(GetHUD());
	bool hasWidget = false;
	if (hasWidget == false)
	{
		widget = CreateWidget<UgamePlayUserWidget>(GetWorld(), WidgetClass);
		widget->AddToViewport();
		hasWidget = true;
	}*/
	//UgamePlayUserWidget* testWidget = nullptr;
	//HUD = Cast<AgamePlayUserHUD>(GetHUD());
	//if (HUD != nullptr)
	//{
	//	widget = Cast<UgamePlayUserWidget>(HUD->widget);
	//	//FString thingyToPrint = widget->GetName();
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
	//}

	//if (widget != nullptr)
	//{
	//	bool hasIcon = false;
	//	if (!hasIcon)
	//	{
	//		UPanelWidget* horBox = (UPanelWidget*)widget->GetWidgetFromName(TEXT("PlayerIcons"));
	//		TArray<AActor*> allFighters;
	//		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFighterBase::StaticClass(), allFighters);
	//		for (int32 i = 0; i < allFighters.Num(); i++)
	//		{
	//			icon = CreateWidget<UPlayerIconWidget>(GetWorld(), iconWidget);
	//			horBox->AddChild(icon);
	//			//hasIcon = true;
	//		}
	//	}
		/*UPanelWidget* horBox = (UPanelWidget*)widget->GetWidgetFromName(TEXT("PlayerIcons"));
		for (int32 i = 0; i <= FighterBases.Num(); i++)
		{
			icon = CreateWidget<UPlayerIconWidget>(GetWorld(), iconWidget);
			horBox->AddChild(icon);
		}*/

		//bool hasIcon = false;
		//if (!hasIcon)
		//{

		//	/*icon = CreateWidget<UPlayerIconWidget>(GetWorld(), iconWidget);
		//	UPanelWidget* horBox = (UPanelWidget*)widget->GetWidgetFromName(TEXT("PlayerIcons"));
		//	horBox->AddChild(icon);*/
		//	hasIcon = true;
		//}
	//}
}

void AFighterController::RunGamePlayWidget()
{
	SpawnGameplayWidgets();
}

bool AFighterController::UpdatePlayerHealth_Validate()
{
	return true;
}

void AFighterController::UpdatePlayerHealth_Implementation()
{
	if (allIcons.Num() >= 2)
	{
		/*AFighterBase* Pawn = Cast<AFighterBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Pawn != nullptr) 
		{
			yourIcon->SetHP(Pawn->GetHealth());
			yourIcon->SetAggression(Pawn->GetAggression());
			yourIcon->SetComboNumber(Pawn->GetBarsBuilt());
		}*/
		for (int32 i = 0; i < allIcons.Num(); i++)
		{
			AFighterBase* Pawn = Cast<AFighterBase>(allFighters[i]);
			if (Pawn != nullptr && allIcons[i] != nullptr)
			{
				allIcons[i]->SetHP(Pawn->GetHealth());
				allIcons[i]->SetAggression(Pawn->GetAggression());
				allIcons[i]->SetComboNumber(Pawn->GetBarsBuilt());
			}
		}
	}
}

void AFighterController::BeginPlay()
{
	Super::BeginPlay();

	//UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	//InstantiateStageSwitchWipeWidget();

	//SetSliderVals();


	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	//Initialize_RemoveAllWidgets();

}

void AFighterController::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	PlayTransitionSound();
}


void AFighterController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	//Prevents single player mode from spawning transition screen:
	if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "Stage1"
		|| UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "CharacterSelection"
		|| UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "LeaderboardMap")
	{
		if (!isSpectating)
		{
			//Try it here?
			Initialize_RemoveAllWidgets();

			InstantiateStageSwitchWipeWidget(false, true);

			Server_SetStageSwitchWipeWidgetNull(false);
		}
	}
}

void AFighterController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFighterController, hasWiped);
	DOREPLIFETIME(AFighterController, isStageSwitchWipeWidgetNull);
	DOREPLIFETIME(AFighterController, isSpectating);
	DOREPLIFETIME(AFighterController, FighterBases);

	DOREPLIFETIME(AFighterController, UI_PlayerName);
	DOREPLIFETIME(AFighterController, hasUI);

}


bool AFighterController::SetHasWiped_Validate()
{
	return true;
}


void AFighterController::SetHasWiped_Implementation()
{
	if (IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
	{
		if (StageSwitchWipeWidget != nullptr)
		{
			Server_SetHasWiped(StageSwitchWipeWidget->hasWiped);
		}
	}
}

bool AFighterController::Server_SetHasWiped_Validate(bool wiped)
{
	return true;
}


void AFighterController::Server_SetHasWiped_Implementation(bool wiped)
{
	if (Role = ROLE_Authority)
	{
		hasWiped = wiped;
	}
}

bool AFighterController::DestroyWipeWidget_Validate()
{
	return true;
}


void AFighterController::DestroyWipeWidget_Implementation()
{
	if (IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
	{
		if (StageSwitchWipeWidget != nullptr)
		{
			StageSwitchWipeWidget->RemoveFromParent();
			StageSwitchWipeWidget = nullptr;
		}
	}

	Server_SetStageSwitchWipeWidgetNull(false);
}

bool AFighterController::Server_SetStageSwitchWipeWidgetNull_Validate(bool isNull)
{
	return true;
}


void AFighterController::Server_SetStageSwitchWipeWidgetNull_Implementation(bool isNull)
{
	if (Role = ROLE_Authority)
	{
		isStageSwitchWipeWidgetNull = isNull;
	}
}


bool AFighterController::Trigger_WidgetOnTickWipe_Validate()
{
	return true;
}

void AFighterController::Trigger_WidgetOnTickWipe_Implementation()
{
	//if (IsLocalController())
	//{
	//server-side pawn widget too...
		if (StageSwitchWipeWidget != nullptr)
		{
			StageSwitchWipeWidget->isOnTickWipeActivated = true;
		}
	//}
}


bool AFighterController::Server_WidgetOnTickWipe_Validate()
{
	return true;
}

void AFighterController::Server_WidgetOnTickWipe_Implementation()
{
	if (Role == ROLE_Authority)
	{
		Trigger_WidgetOnTickWipe();
	}
}
bool AFighterController::InstantiateMiniLeaderboardWidget_Validate()
{
	return true;
}

void AFighterController::InstantiateMiniLeaderboardWidget_Implementation()
{
	if (MiniLeaderboardWidget == nullptr && MiniLeaderboardWidgetClass != nullptr)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			MiniLeaderboardWidget = CreateWidget<UMiniLeaderboardWidget>(World, MiniLeaderboardWidgetClass);
			MiniLeaderboardWidget->AddToViewport();
		}
	}

	if (hasUI == false)
	{
		HUD = Cast<AgamePlayUserHUD>(GetHUD());
		UWorld* world = GetWorld();
		if (HUD != nullptr)
		{
			widget = Cast<UgamePlayUserWidget>(HUD->widget);
		}
		if (icon == nullptr && iconWidget != nullptr && widget != nullptr)
		{
			if (world != nullptr)
			{
				UGameplayStatics::GetAllActorsOfClass(world, AFighterBase::StaticClass(), allFighters);
				UPanelWidget* horBox = (UPanelWidget*)widget->GetWidgetFromName(TEXT("PlayerIcons"));
				if (allFighters.Num() > 0)
				{
					for (int32 i = 0; i < allFighters.Num(); i++)
					{
						icon = CreateWidget<UPlayerIconWidget>(world, iconWidget);
						allIcons.Add(icon);
						horBox->AddChild(icon);
						AFighterBase* Pawn = Cast<AFighterBase>(allFighters[i]);
						if (Pawn != nullptr)
						{
							if (Pawn->ActorHasTag(FName(TEXT("Demark"))))
							{
								allIcons[i]->SetPlayerImage(1);
							}
							else if (Pawn->ActorHasTag(FName(TEXT("Maskium"))))
							{
								allIcons[i]->SetPlayerImage(0);
							}

							//=====Set Widget properties:
							int pIndex = -1;
							//Set Color:

							AFighterPlayerState* FPS = Cast<AFighterPlayerState>(Pawn->GetPlayerState());
							if (FPS != nullptr)
							{
								//set pindex for later usage:
								pIndex = FPS->PlayerIndex;

								allIcons[i]->SetImageColor(pIndex);
								allIcons[i]->SetName(FText::FromString(FPS->GetPlayerName()));
							}

							//Set Arrow:
							AFighterController* FC = Cast<AFighterController>(Pawn->GetController());
							if (FC != nullptr)
							{
								if (FC == this)
								{
									//If the controller is in charge of the current pawn in the array, set the image:
									allIcons[i]->SetArrowVisibility(pIndex);
								}
							}


							//============================
						}
						if (i == allFighters.Num() - 1)
						{
							hasUI = true;
						}
					}
				}
			}
		}
	}
}

bool AFighterController::MakeUserHudLarge_Validate(UPlayerIconWidget* icon)
{
	return true;
}

void AFighterController::MakeUserHudLarge_Implementation(UPlayerIconWidget* icon)
{
	if (icon != nullptr)
	{
		icon->SetRenderScale(FVector2D(1.5f, 1.5));
	}
}

bool AFighterController::InstantiateTimerScreen_Validate()
{
	return true;
}

void AFighterController::InstantiateTimerScreen_Implementation()
{
	if (IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
	{ 
		if (TimerScreen == nullptr && TimerScreenClass != nullptr)
		{
			UWorld* World = GetWorld();
			if (World != nullptr)
			{
				TimerScreen = CreateWidget<UTimerScreen>(World, TimerScreenClass);
				TimerScreen->AddToViewport();
			}
		}
	}
}

bool AFighterController::DestroyMiniLeaderboardWidget_Validate()
{
	return true;
}

void AFighterController::DestroyMiniLeaderboardWidget_Implementation()
{
	if (MiniLeaderboardWidget != nullptr)
	{
		if (MiniLeaderboardWidget->GetParent() != nullptr)
		{
			MiniLeaderboardWidget->RemoveFromParent();
		}
		MiniLeaderboardWidget = nullptr;
	}
}

bool AFighterController::Initialize_RemoveAllWidgets_Validate()
{
	return true;
}

void AFighterController::Initialize_RemoveAllWidgets_Implementation()
{
	TArray<UUserWidget*> widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), widgets, UStageSwitchWipeWidget::StaticClass(), true);
	for (int j = 0; j < widgets.Num(); j++)
	{
		widgets[j]->RemoveFromParent();
	}
}

bool AFighterController::Multicast_GetSurvivingPlayers_Validate()
{
	return true;
}

void AFighterController::Multicast_GetSurvivingPlayers_Implementation()
{
	FighterBases.Empty();

	AFightingGameState* FGS = GetWorld()->GetGameState<AFightingGameState>();
	if (FGS != nullptr)
	{
		for (int i = 0; i < FGS->survivingPlayers.Num(); i++)
		{
			AFighterBase* FB = FGS->survivingPlayers[i];
			if (FB != nullptr)
			{

				FighterBases.Add(FB);
			}
		}
	}

}

void AFighterController::Trigger_GetPlayerName()
{
	Server_GetPlayerName();
}

bool AFighterController::Server_GetPlayerName_Validate()
{
	return true;
}

void AFighterController::Server_GetPlayerName_Implementation()
{
	AFighterPlayerState* FPS = GetPlayerState<AFighterPlayerState>();
	if (FPS != nullptr)
	{
		UI_PlayerName = FPS->GetPlayerName();
	}
	else
	{
		UI_PlayerName = "NULL";
	}
}

bool AFighterController::PlayTransitionSound_Validate()
{
	return true;
}

void AFighterController::PlayTransitionSound_Implementation()
{
	//UAkGameplayStatics::PostEvent(TransitionEvent, this, false, "");
}

bool AFighterController::Debug_RemoveAllWidgets_Validate()
{
	return true;
}

void AFighterController::Debug_RemoveAllWidgets_Implementation()
{
	AFightingGameState* FGS = GetWorld()->GetGameState<AFightingGameState>();
	if (FGS != nullptr)
	{
		if (FGS->activateDevMode)
		{
			//UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		}
	}
}

bool AFighterController::Multicast_SetTransitionScreenTransparent_Validate()
{
	return true;
}

void AFighterController::Multicast_SetTransitionScreenTransparent_Implementation()
{
	if (IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
	{
		if (StageSwitchWipeWidget != nullptr)
		{
			AFightingGameState* FGS = GetWorld()->GetGameState<AFightingGameState>();
			if (FGS != nullptr)
			{
				if (FGS->activateDevMode && FGS->timerState == ETimerState::InitialCountdown)
				{
					SetTransitionScreenTransparent();
				}
			}
		}
	}
}

void AFighterController::SetTransitionScreenTransparent()
{
	if (IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
	{
		UStageSwitchWipeWidget* myWidget = StageSwitchWipeWidget;
		if (IsValid(myWidget) && myWidget->IsValidLowLevel())
		{
			
			myWidget->SetTransitionScreenTransparent();
		}
	}
}

bool AFighterController::PlayNovaRoyaleSound_Validate()
{
	return true;
}

void AFighterController::PlayNovaRoyaleSound_Implementation()
{
	UAkGameplayStatics::PostEvent(NovaRoyaleEvent, GetWorld()->GetGameState<AFightingGameState>(), false, "");

}
bool AFighterController::InstantiateEliminationWidget_Validate()
{
	return true;
}

void AFighterController::InstantiateEliminationWidget_Implementation()
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	UWorld* World = GetWorld();
	if ((IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
		&& EliminationWidget == nullptr)
	{
		if (EliminationWidgetClass != nullptr)
		{
			EliminationWidget = CreateWidget<UEliminationWidget>(World, EliminationWidgetClass);

			EliminationWidget->AddToViewport();
		}
	}
}

bool AFighterController::InstantiateStageChangeWidget_Validate()
{
	return true;
}

void AFighterController::InstantiateStageChangeWidget_Implementation()
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
	{
		if (StageChangeWidgetClass != nullptr)
		{
			UStageChangeWidget* StageChangeWidget = CreateWidget<UStageChangeWidget>(World, StageChangeWidgetClass);

			StageChangeWidget->AddToViewport();
		}
	}
}
bool AFighterController::Multicast_GetPlayerName_Validate()
{
	return true;
}

void AFighterController::Multicast_GetPlayerName_Implementation()
{
	if (PlayerName == "")
	{
		if (GetPlayerState<AFighterPlayerState>() != nullptr)
		{
			PlayerName = GetPlayerState<AFighterPlayerState>()->GetPlayerName();
		}
	}
}

bool AFighterController::MiniLeaderboard_GetPlayerName_Validate()
{
	return true;
}

void AFighterController::MiniLeaderboard_GetPlayerName_Implementation()
{
	if (MiniLeaderboardWidget != nullptr)
	{
		MiniLeaderboardWidget->PlayerName = PlayerName;
	}
}

bool AFighterController::MiniLeaderboard_FetchRanking_Validate(int ranking)
{
	return true;
}

void AFighterController::MiniLeaderboard_FetchRanking_Implementation(int ranking)
{
	AFightingGameState* FGS = GetWorld()->GetGameState<AFightingGameState>();
	if (FGS != nullptr)
	{
		FGS->Wwise_PlayerRanking = ranking;
	}
}

bool AFighterController::Server_GetTimer_Validate()
{
	return true;
}

void AFighterController::Server_GetTimer_Implementation()
{
	float val = 0;
	AFightingGameState* FGS = GetWorld()->GetGameState<AFightingGameState>();
	if (FGS != nullptr)
	{
		val = FGS->timer;
	}
	Client_GetTimer(val);
}

void AFighterController::Client_GetTimer_Implementation(float val)
{
	if (TimerScreen != nullptr)
	{
		TimerScreen->Timer = val;
	}
}


bool AFighterController::Server_GetTimerState_Validate()
{
	return true;
}

void AFighterController::Server_GetTimerState_Implementation()
{
	ETimerState val = ETimerState::Invalid;
	AFightingGameState* FGS = GetWorld()->GetGameState<AFightingGameState>();
	if (FGS != nullptr)
	{
		val = FGS->timerState;
	}
	Client_GetTimerState(val);
}

void AFighterController::Client_GetTimerState_Implementation(ETimerState val)
{
	if (TimerScreen != nullptr)
	{
		TimerScreen->TimerState = val;
	}
}

//Cannot be multicast for incrementing server variables... has to split both server and client based functions, or else the number incremented would mess up!
bool AFighterController::Server_IncrementNumOfStageCompleteScreensAnimsPlayed_Validate()
{
	return true;
}

void AFighterController::Server_IncrementNumOfStageCompleteScreensAnimsPlayed_Implementation()
{
	AFightingGameState* FGS = GetWorld()->GetGameState<AFightingGameState>();
	if (FGS != nullptr)
	{
		FGS->numOfStageCompleteScreensAnimsPlayed += 1;
	}
}

void AFighterController::Client_IncrementNumOfStageCompleteScreensAnimsPlayed_Implementation()
{
	Server_IncrementNumOfStageCompleteScreensAnimsPlayed();
}

bool AFighterController::SetEliminationWidgetNullptr_Validate()
{
	return true;
}

void AFighterController::SetEliminationWidgetNullptr_Implementation()
{
	if (IsLocalController() || GetNetMode() == ENetMode::NM_ListenServer)
	{
		if (EliminationWidget != nullptr && TimerScreen != nullptr)
		{
			if (TimerScreen->TimerState == ETimerState::InitialCountdown)
			{
				//dereference it here, so that the prev widget can return the correct value for anim done...
				EliminationWidget->RemoveFromParent();
				if (EliminationWidget != nullptr)
				{
					EliminationWidget = nullptr;
				}
			}
		}
	}
}