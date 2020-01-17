// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "FindSessionsCallbackProxy.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"
#include "ServerList.generated.h"


struct FServerData;
class UNetworkingGameInstance;
class ULoginScreen;
class UServerRow;

/**
 * 
 */

//Establish a connection between bp and cpp for fetching session results
USTRUCT(BlueprintType)
struct NOVAFINAL_API FBPCPPSessionResult
{
	GENERATED_USTRUCT_BODY()

	FOnlineSessionSearchResult sessionSearchResult;
};

UCLASS()
class NOVAFINAL_API UServerList : public UUserWidget
{
	GENERATED_BODY()

private:
	void SetFindingExistingServerText(float DeltaTime);

	UFUNCTION()
	void Back();

	UFUNCTION()
	void Refresh();

	void CreateServerRow(TArray<FServerData> arrOfServerRowData);

	float timer = 0;
	float speed = 3;

	bool isSearching = false;

	const FString findServerText = "Finding Existing Servers";

	UEditableTextBox* findingServerTextBox;
	UButton* backButton;
	UButton* refreshButton;
	UScrollBox* scrollingServers;

	ULoginScreen* loginScreen;

	TArray<UServerRow*> FoundSessionWidgets;


protected:

	/** Assigns passed FText to DisplayText */
	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetStatusMessage(const FText& NewStatusMessage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
		TSubclassOf<class ULoginScreen> LoginScreenClass;

	// Called when the game starts or when spawned
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "FindRoomsAndHandleRoomData_BPImplementation"))
		//TArray<FBPCPPSessionResult> FindRoomsAndHandleRoomData_BPImplementation(APlayerController* OwningPlayer);
		void FindRoomsAndHandleRoomData_BPImplementation(APlayerController* OwningPlayer);

public:
	UPROPERTY(EditAnywhere)
	TArray<FBPCPPSessionResult> FoundSessions;

	
};
