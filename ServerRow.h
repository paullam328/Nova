// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSessionSettings.h"
#include "FindSessionsCallbackProxy.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UServerRow : public UUserWidget
{
	GENERATED_BODY()


private:
	UFUNCTION()
		void JoinRoom();

protected:
	UEditableTextBox* serverNameTextBox;
	UButton* joinRoomButton;

	// Called when the game starts or when spawned
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "JoinSession_BPImplementation"))
		void JoinSession_BPImplementation(APlayerController* PlayerController, FBlueprintSessionResult BlueprintSessionResult);
};
