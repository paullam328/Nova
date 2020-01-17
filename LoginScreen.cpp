// Copywrite Shapeshifter 2019

#include "LoginScreen.h"
#include "WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "ServerList.h"
#include "RoomNamePrompt.h"
#include "AkComponent.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"

void ULoginScreen::Play()
{
	playButton->SetVisibility(ESlateVisibility::Hidden);
	findExistingRoomButton->SetVisibility(ESlateVisibility::Visible);
	createRoomButton->SetVisibility(ESlateVisibility::Visible);

	findExistingRoomButton->GetChildAt(0)->SetVisibility(ESlateVisibility::Visible);
	createRoomButton->GetChildAt(0)->SetVisibility(ESlateVisibility::Visible);
}

void ULoginScreen::FindExistingRoom()
{
	if (ServerListClass)
	{
		serverList = CreateWidget<UServerList>(GetWorld(), ServerListClass);
		serverList->AddToViewport();
	}
	RemoveFromParent();
}

void ULoginScreen::CreateRoom()
{
	if (RoomNamePromptClass)
	{
		roomNamePrompt = CreateWidget<URoomNamePrompt>(GetWorld(), RoomNamePromptClass);
		roomNamePrompt->AddToViewport();
	}
}

void ULoginScreen::NativeConstruct()
{
	playButton = (UButton*)(WidgetTree->FindWidget(FName("PlayButton")));
	if (playButton)
	{
		playButton->OnClicked.AddDynamic(this, &ULoginScreen::Play);
	}

	findExistingRoomButton = (UButton*)(WidgetTree->FindWidget(FName("FindExistingRoom")));
	if (findExistingRoomButton)
	{
		findExistingRoomButton->OnClicked.AddDynamic(this, &ULoginScreen::FindExistingRoom);
	}

	createRoomButton = (UButton*)(WidgetTree->FindWidget(FName("CreateRoom")));
	if (createRoomButton)
	{
		createRoomButton->OnClicked.AddDynamic(this, &ULoginScreen::CreateRoom);
	}

	//bShowMouseCursor should be false after the gamepad menu navigation is implemented:
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	playButton->SetVisibility(ESlateVisibility::Visible);
	findExistingRoomButton->SetVisibility(ESlateVisibility::Hidden);
	createRoomButton->SetVisibility(ESlateVisibility::Hidden);

	//UAkComponent::PostAkEventByName("Play_FE_MUS");
	//Cast<UAkComponent>(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetComponentByClass(UAkComponent::StaticClass()))->PostAkEvent(AkEvent, "");
	//UAkGameplayStatics::PostEvent(AkEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), false, "");
}

