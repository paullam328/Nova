// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

#include "LoginScreen.generated.h"

/**
 * 
 */
class UServerList;
class URoomNamePrompt;
class UAkComponent;
class UAkAudioEvent;

UCLASS()
class NOVAFINAL_API ULoginScreen : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UEditableTextBox* NovaTextBox;
	UButton* playButton;
	UButton* findExistingRoomButton;
	UButton* createRoomButton;

	UServerList* serverList;
	URoomNamePrompt* roomNamePrompt;

	UFUNCTION()
		void Play();

	UFUNCTION()
		void FindExistingRoom();

	UFUNCTION()
		void CreateRoom();


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
		TSubclassOf<class UServerList> ServerListClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
		TSubclassOf<class URoomNamePrompt> RoomNamePromptClass;

	//AK Events:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * AkEvent;


	// Called when the game starts or when spawned
	virtual void NativeConstruct() override;

};
