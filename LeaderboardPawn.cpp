// Copywrite Shapeshifter 2019

#include "LeaderboardPawn.h"
#include "LeaderboardWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "NetworkingGameInstance.h"
#include "StageSwitchWipeWidget.h"

// Sets default values
ALeaderboardPawn::ALeaderboardPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALeaderboardPawn::BeginPlay()
{
	Super::BeginPlay();

	SetMusicVolume();

	InitializeTransitionWidget();

	RetrieveAndRenderMatchInfo();

	TArray<UActorComponent*> SKC = GetComponentsByTag(USkeletalMesh::StaticClass(), "Char");

}

// Called every frame
void ALeaderboardPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		if (StageSwitchWipeWidget != nullptr)
		{
			StageSwitchWipeWidget->Wipe(false);
		}
	}
}

// Called to bind functionality to input
void ALeaderboardPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ALeaderboardPawn::RetrieveAndRenderMatchInfo_Validate()
{
	return true;
}

void ALeaderboardPawn::RetrieveAndRenderMatchInfo_Implementation()
{
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			UNetworkingGameInstance* NGI = Cast<UNetworkingGameInstance>(UGameplayStatics::GetGameInstance(World));
			SetLeaderboardText(NGI->LeaderboardDataStack);
		}
	}
}

bool ALeaderboardPawn::SetLeaderboardText_Validate(const TArray<FLeaderboardData>& data)
{
	return true;
}

void ALeaderboardPawn::SetLeaderboardText_Implementation(const TArray<FLeaderboardData>& data)
{
	if (IsLocallyControlled())
	{
		LeaderboardWidget->SetLeaderboardText(data);
	}
}

void ALeaderboardPawn::Trigger_KickMeOut()
{
	KickMeOut();
}

bool ALeaderboardPawn::KickMeOut_Validate()
{
	return true;
}

void ALeaderboardPawn::KickMeOut_Implementation()
{
	//YASSINE DO SOMETHING FOR FUCK SAKE

}

bool ALeaderboardPawn::InitializeTransitionWidget_Validate()
{
	return true;
}

void ALeaderboardPawn::InitializeTransitionWidget_Implementation()
{
	if (IsLocallyControlled())
	{
		UWorld* World = GetWorld();
		if (World)
		{

			if (LeaderboardWidget == nullptr && LeaderboardWidgetClass != nullptr)
			{
				LeaderboardWidget = CreateWidget<ULeaderboardWidget>(World, LeaderboardWidgetClass);
				LeaderboardWidget->AddToViewport();
				//APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
			}

			if (StageSwitchWipeWidget == nullptr && StageSwitchWipeWidgetClass != nullptr)
			{
				StageSwitchWipeWidget = CreateWidget<UStageSwitchWipeWidget>(World, StageSwitchWipeWidgetClass);
				StageSwitchWipeWidget->AddToViewport();
				StageSwitchWipeWidget->SetDisolve(2.0f);
				//StageSwitchWipeWidget->FactText->SetVisibility(ESlateVisibility::Visible);
				//StageSwitchWipeWidget->Text->SetVisibility(ESlateVisibility::Visible);
				StageSwitchWipeWidget->isTextInitialized = true;
				//APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
			}
		}
	}
}

void ALeaderboardPawn::SetMusicVolume()
{
	UNetworkingGameInstance* NGI = GetGameInstance<UNetworkingGameInstance>();
	if (NGI != nullptr)
	{
		UAkGameplayStatics::SetOutputBusVolume(NGI->MusSliderVal, this);
	}
}