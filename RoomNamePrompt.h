// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "RoomNamePrompt.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API URoomNamePrompt : public UUserWidget
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void Confirm();
	
	UFUNCTION()
	void Cancel();

	UMultiLineEditableTextBox* roomNameTextBox;
	UButton* confirmButton;
	UButton* cancelButton;

protected:

	// Called when the game starts or when spawned
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CreateRoom_BPImplementation"))
		void CreateRoom_BPImplementation(const FString& LevelName);
};
