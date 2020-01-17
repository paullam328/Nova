// Copywrite Shapeshifter 2019

#include "ServerRow.h"
#include "ServerList.h"
#include "WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

void UServerRow::NativeConstruct()
{
	Super::NativeConstruct();

	joinRoomButton = (UButton*)(WidgetTree->FindWidget(FName("JoinRoomButton")));
	if (joinRoomButton)
	{
		joinRoomButton->OnClicked.AddDynamic(this, &UServerRow::JoinRoom);
	}

	serverNameTextBox = Cast<UEditableTextBox>(WidgetTree->FindWidget(FName("ServerNameText")));

}

void UServerRow::JoinRoom()
{
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UServerList::StaticClass(), true);
	
	//Uncomment this later:
	/*TArray<FBlueprintSessionResult> FoundSessions = Cast<UServerList>(FoundWidgets[0])->FoundSessions;
	for (int i = 0; i < FoundSessions.Num(); i++)
	{
		JoinSession_BPImplementation(UWidget::GetOwningPlayer(), FoundSessions[i]);
	}*/
}