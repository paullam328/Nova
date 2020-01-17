// Copywrite Shapeshifter 2019

#include "CharacterSelectPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine.h"
#include "CharacterSelectWidget.h"
#include "CharacterSelectGameState.h"

// Sets default values
ACharacterSelectPawn::ACharacterSelectPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ACharacterSelectPawn::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		if (IsLocallyControlled())
		{
			if (CharacterSelectWidget == nullptr && CharacterSelectWidgetClass != nullptr)
			{
				FillArray();

				CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(World, CharacterSelectWidgetClass);
				CharacterSelectWidget->AddToViewport();
				//UGameplayStatics::GetPlayerController(World, 0)->SetInputMode();
				APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, CharacterSelectWidget->Character1Button, EMouseLockMode::DoNotLock);



				TArray<AActor*> arrOfCams;
				UGameplayStatics::GetAllActorsOfClass(World, ACameraActor::StaticClass(), arrOfCams);
				ACameraActor* cam = Cast<ACameraActor>(arrOfCams[0]);
				PC->SetViewTargetWithBlend(cam, 0.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);



			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Widget is detached from CharacterSelectGameManager_BP.  Please re-attach them!")));
			}
		}
	}
}

// Called every frame
void ACharacterSelectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterSelectWidget != nullptr)
	{
		SetNum(CharacterSelectWidget->CharNum);

		for (int i = 0; i < allModels.Num(); i++)
		{
			if (i == num)
			{
				allModels[i]->GetSkeletalMeshComponent()->SetVisibility(true);
			}
			else
			{
				allModels[i]->GetSkeletalMeshComponent()->SetVisibility(false);
			}
		}
	}
	if (GetWorld())
	{
		SetPlayerNotReady();
		SetMyPlayerName();
		UpdateSelectArray();
		UpdatePlayersReady();
	}
}

void ACharacterSelectPawn::FillArray()
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

// Called to bind functionality to input
void ACharacterSelectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterSelectPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterSelectPawn, charEnum);
	DOREPLIFETIME(ACharacterSelectPawn, PlayerIndex);
	DOREPLIFETIME(ACharacterSelectPawn, num);
	DOREPLIFETIME(ACharacterSelectPawn, arrOfSelectedCharIndices);
	DOREPLIFETIME(ACharacterSelectPawn, IsIndexSet);

	DOREPLIFETIME(ACharacterSelectPawn, arrOfPlayersReady);
	DOREPLIFETIME(ACharacterSelectPawn, PlayerReadyState);
}

bool ACharacterSelectPawn::SetCharEnumSelected_Validate()
{
	return true;
}

void ACharacterSelectPawn::SetCharEnumSelected_Implementation()
{
	if (Role == ROLE_Authority)
	{
		switch (num + 1)
		{
		case 1:
			charEnum = ECharacters::Demark;
			break;
		case 2:
			charEnum = ECharacters::Maskium;
			break;
		default:
			break;
		}
	}
}

void ACharacterSelectPawn::SetMyPlayerName()
{
	//if (Role == ROLE_Authority && IsLocallyControlled())
	//{ 
		if (PlayerIndex > -1 && CharacterSelectWidget != nullptr)
		{
			if (CharacterSelectWidget->PlayerNames.IsValidIndex(0)
				&& CharacterSelectWidget->PlayerNames.Num() == 6)
			{
				CharacterSelectWidget->PlayerNames[PlayerIndex]->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)));
			}
		}
	//}
}

bool ACharacterSelectPawn::SetNum_Validate(int CharNum)
{
	return true;
}

void ACharacterSelectPawn::SetNum_Implementation(int CharNum)
{
	if (Role == ROLE_Authority)
	{
		num = CharNum;
	}
}


bool ACharacterSelectPawn::UpdateSelectArray_Validate()
{
	return true;
}

void ACharacterSelectPawn::UpdateSelectArray_Implementation()
{
	if (Role == ROLE_Authority)
	{
		ACharacterSelectGameState* GS = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());
		if (GS != nullptr)
		{
			if (!arrOfSelectedCharIndices.IsValidIndex(0)
				|| arrOfSelectedCharIndices.Num() != 6)
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

bool ACharacterSelectPawn::UpdatePlayersReady_Validate()
{
	return true;
}

void ACharacterSelectPawn::UpdatePlayersReady_Implementation()
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

bool ACharacterSelectPawn::ChangeSelectImg_Validate()
{
	return true;
}
void ACharacterSelectPawn::ChangeSelectImg_Implementation()
{
	if (Role == ROLE_Authority)
	{
		SetCharEnumSelected();
		//Cast<ACharacterSelectGameState>(GetWorld()->GetGameState())->SetSelectedCharPerPlayer(PlayerIndex, charEnum);
	}
}

void ACharacterSelectPawn::Trigger_ChangeSelectImg()
{
	ChangeSelectImg();
}

bool ACharacterSelectPawn::ChangePlayerReady_Validate()
{
	return true;
}

void ACharacterSelectPawn::ChangePlayerReady_Implementation()
{
	if (Role == ROLE_Authority)
	{
		Cast<ACharacterSelectGameState>(GetWorld()->GetGameState())->SetPlayerReadyPerPlayer(PlayerIndex);
	}
}

void ACharacterSelectPawn::Trigger_PlayerReady()
{
	ChangePlayerReady();
}

bool ACharacterSelectPawn::SetPlayerNotReady_Validate()
{
	return true;
}

void ACharacterSelectPawn::SetPlayerNotReady_Implementation()
{
	if (Role == ROLE_Authority)
	{
		Cast<ACharacterSelectGameState>(GetWorld()->GetGameState())->SetPlayerNotReadyPerPlayer(PlayerIndex);
	}
}

bool ACharacterSelectPawn::TravelPlayer_Validate()
{
	return true;
}

void ACharacterSelectPawn::TravelPlayer_Implementation()
{
	if (Role == ROLE_Authority)
	{
		FString nMapName = "Stage1";
		UGameplayStatics::OpenLevel(GetWorld(), FName(*nMapName), true, "listen");	
	}
}