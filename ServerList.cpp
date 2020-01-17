// Copywrite Shapeshifter 2019

#include "ServerList.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkingGameInstance.h"
#include "WidgetTree.h"
#include "LoginScreen.h"
#include "ServerRow.h"

void UServerList::Back()
{
	if (LoginScreenClass)
	{
		loginScreen = CreateWidget<ULoginScreen>(GetWorld(), LoginScreenClass);
		loginScreen->AddToViewport();
	}
	RemoveFromParent();
}

void UServerList::Refresh()
{
	isSearching = true;
	findingServerTextBox->SetVisibility(ESlateVisibility::Visible);

	//Delete Existing Server Rows:
	for (int i = 0; i < FoundSessionWidgets.Num(); i++)
	{
		scrollingServers->RemoveChild(FoundSessionWidgets[i]);
	}
	FoundSessionWidgets.Empty();
	scrollingServers->ClearChildren();

	//Update List On Tick:
	UNetworkingGameInstance* networkGameIns = Cast<UNetworkingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//TArray<FBPCPPSessionResult> FoundSessions = FindRoomsAndHandleRoomData_BPImplementation(UWidget::GetOwningPlayer());
	FindRoomsAndHandleRoomData_BPImplementation(UWidget::GetOwningPlayer());
	for (int i = 0; i < FoundSessions.Num(); i++)
	{
		FServerData serverData;
		serverData.sessionName = FoundSessions[i].sessionSearchResult.Session.OwningUserName;
		serverData.numOfPlayers = FoundSessions[i].sessionSearchResult.Session.SessionSettings.NumPublicConnections - FoundSessions[i].sessionSearchResult.Session.NumOpenPublicConnections;
		serverData.maxNumOfPlayers = FoundSessions[i].sessionSearchResult.Session.SessionSettings.NumPublicConnections;
		networkGameIns->arrOfServerRowData.Add(serverData);
	}
	networkGameIns->bStartUpdateServerRowData = true;


}

void UServerList::CreateServerRow(TArray<FServerData> arrOfServerRowData)
{
	isSearching = false;
	if (arrOfServerRowData.Num() > 0)
	{
		findingServerTextBox->SetVisibility(ESlateVisibility::Collapsed);
		for (int i = 0; i < arrOfServerRowData.Num(); i++)
		{
			//arrOfServerRowData[i].serverName
			//scrollingServers->AddChild(arrOfServerRowData[i]);
		}
	}
	else
	{
		if (WidgetTree && findingServerTextBox)
		{
			findingServerTextBox->SetText(FText::FromString("No Session Found"));
		}
	}
}

/** Assigns passed FText to DisplayText */
void UServerList::SetStatusMessage(const FText& NewStatusMessage)
{
	if (WidgetTree && findingServerTextBox)
	{
		findingServerTextBox->SetText(NewStatusMessage);
	}
}

void UServerList::NativeConstruct()
{
	Super::NativeConstruct();

	backButton = (UButton*)(WidgetTree->FindWidget(FName("BackButton")));
	if (backButton)
	{
		backButton->OnClicked.AddDynamic(this, &UServerList::Back);
	}

	refreshButton = (UButton*)(WidgetTree->FindWidget(FName("RefreshButton")));
	if (refreshButton)
	{
		//Add later
		refreshButton->OnClicked.AddDynamic(this, &UServerList::Refresh);
	}

	findingServerTextBox = Cast<UEditableTextBox>(WidgetTree->FindWidget(FName("FindingServerText")));

	scrollingServers = (UScrollBox*)(WidgetTree->FindWidget(FName("ScrollingServers")));

	Refresh();
}

void UServerList::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (isSearching)
	{
		SetFindingExistingServerText(DeltaTime);
	}

	if (GetWorld())
	{
		UWorld* world = GetWorld();
		if (Cast<UNetworkingGameInstance>(UGameplayStatics::GetGameInstance(world)))
		{
			UNetworkingGameInstance* networkGameIns = Cast<UNetworkingGameInstance>(UGameplayStatics::GetGameInstance(world));
			if (networkGameIns->bStartUpdateServerRowData)
			{
				TArray<FServerData> arrOfServerRowData = networkGameIns->UpdateArrOfServerRowData();
				CreateServerRow(arrOfServerRowData);
				networkGameIns->bStartUpdateServerRowData = false;
			}
		}
	}

	
}


void UServerList::SetFindingExistingServerText(float DeltaTime)
{
	timer = DeltaTime * speed;

	FString NewServerText = "";
	switch (FMath::RoundToInt(timer) % 4)
	{
		case 0:
			NewServerText = findServerText;
			break;
		case 1:
			NewServerText = findServerText + ".";
			break;
		case 2:
			NewServerText = findServerText + "..";
			break;
		case 3:
			NewServerText = findServerText + "...";
			break;

		default:
			break;

	}
	SetStatusMessage(FText::FromString(NewServerText));
}