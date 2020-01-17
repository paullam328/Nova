// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Animation/SkeletalMeshActor.h"
#include "UnrealNetwork.h"
#include "EPlayerReady.h"
#include "ECharacter.h"
#include "CharacterSelectController.generated.h"



class UCharacterSelectWidget;
class UStageSwitchWipeWidget;
class UAnimSequence;
class UAkAudioEvent;
class UAkComponent;
class UAkGameplayStatics;
class UAnimMontage;


/**
 * 
 */
UCLASS()
class NOVAFINAL_API ACharacterSelectController : public APlayerController
{
	GENERATED_BODY()


		ACharacterSelectController();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void SetupInputComponent() override;

public:

	UFUNCTION()
	void HoldBToLeave();

	UFUNCTION()
	void ReleaseB();

	// The Pre Client Travel see if gets triggered
	void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, WithValidation, Reliable)
		void SetCharEnumSelected();

	UPROPERTY(Replicated)
		ECharacter charEnum;

	UPROPERTY(Replicated)
		int PlayerIndex = -1; //this indicates which player you are

	UPROPERTY(Replicated)
		int num = 0;

	int setNum = -2;

	UFUNCTION(Server, WithValidation, Reliable)
		void Trigger_ServerTravel();


	UPROPERTY(Replicated)
		bool IsIndexSet = false;

	UPROPERTY(BlueprintReadWrite, Category = "TestingInput")
	UCharacterSelectWidget* CharacterSelectWidget;

	UStageSwitchWipeWidget* StageSwitchWipeWidget;

	//Setting selected characters:---------------------------------------
	void Trigger_ChangeSelectImg();

	UFUNCTION(Server, WithValidation, Reliable)
		void ChangeSelectImg();

	//What if, the values are being grabbed from the game tate and passed to the pawn
	// Which means, the one below is more of an output array
	UPROPERTY(Replicated)
		TArray<int> arrOfSelectedCharIndices;

	//Checking whether players are ready:---------------------------------
	void Trigger_PlayerReady();

	UFUNCTION(Server, WithValidation, Reliable)
		void SetPlayerNotReady(); //on login

	UPROPERTY(Replicated)
		EPlayerReady PlayerReadyState = EPlayerReady::NonExistent;

	UFUNCTION(Server, WithValidation, Reliable)
		void ChangePlayerReady();

	//What if, the values are being grabbed from the game state and passed to the pawn
// Which means, the one below is more of an output array
	UPROPERTY(Replicated)
		TArray<EPlayerReady> arrOfPlayersReady;

	//OpenLevel/ServerTravel:--------------------------------------------
	UFUNCTION(Server, WithValidation, Reliable)
		void TravelPlayer();

protected:

	UFUNCTION(Server, WithValidation, Reliable)
		void SetNum(int CharNum);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
		TSubclassOf<UCharacterSelectWidget> CharacterSelectWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
		TSubclassOf<UStageSwitchWipeWidget> StageSwitchWipeWidgetClass;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetMyPlayerName();

	//Setting selected characters:---------------------------------------
	UFUNCTION(Server, WithValidation, Reliable)
		void UpdateSelectArray();

	//Checking whether players are ready:---------------------------------
	UFUNCTION(Server, WithValidation, Reliable)
		void UpdatePlayersReady();


	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void TriggerSetCharacterImages();


private:

	TArray<ASkeletalMeshActor*> allModels;

	void FillArray();

	UFUNCTION()
	void MissClick();

public:
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InstantiateStageSwitchWipeWidget();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Trigger_PlayAlertAnim();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Server_PlayAlertAnim();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void RemoveCharacterSelectionWidget();

	UAkComponent* AkComp;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetAkComp();
	
	//Wwise:
	void AdjustVoiceSlider();

	//For Wwise get player name:
	UPROPERTY(Replicated)
	FString UI_PlayerName = "";

	void Trigger_GetPlayerName();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Server_GetPlayerName();

	//Transition sound:
	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void PlayTransitionSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent* TransitionEvent;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Server_PopupBanner();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetBannerReady();

	UFUNCTION(Client, WithValidation, Reliable)
	void DetectClickStart();

	UFUNCTION(Client, WithValidation, Reliable)
	void DetectBannerClickBack();

	UFUNCTION(Client, WithValidation, Reliable)
	void Client_HideBanner();

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SetNotBannerReady();

	//For Timer in UI:

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Server_GetTimer();

	UFUNCTION(Client, WithValidation, Reliable)
	void Client_GetTimer(float timerSec);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Server_GetShouldShowtimer();

	UFUNCTION(Client, WithValidation, Reliable)
	void Client_GetShouldShowTimer(bool ShouldShowTimer);


	void SelectMaskium();

	void SelectDemark();

};
