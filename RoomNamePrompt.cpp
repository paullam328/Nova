// Copywrite Shapeshifter 2019

#include "RoomNamePrompt.h"
#include "WidgetTree.h"
#include "NetworkingGameInstance.h"
#include "Kismet/GameplayStatics.h"

void URoomNamePrompt::Confirm()
{
	CreateRoom_BPImplementation(roomNameTextBox->GetText().ToString());
}

void URoomNamePrompt::Cancel()
{
	RemoveFromParent();
}

void URoomNamePrompt::NativeConstruct()
{
	confirmButton = (UButton*)(WidgetTree->FindWidget(FName("ConfirmButton")));
	if (confirmButton)
	{
		confirmButton->OnClicked.AddDynamic(this, &URoomNamePrompt::Confirm);
	}

	cancelButton = (UButton*)(WidgetTree->FindWidget(FName("CancelButton")));
	if (cancelButton)
	{
		cancelButton->OnClicked.AddDynamic(this, &URoomNamePrompt::Cancel);
	}

	roomNameTextBox = (UMultiLineEditableTextBox*)(WidgetTree->FindWidget(FName("RoomNameText")));
}