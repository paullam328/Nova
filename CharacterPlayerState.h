// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ECharacter.h"
#include "EPlayerReady.h"
#include "UnrealNetwork.h"
#include "CharacterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API ACharacterPlayerState : public APlayerState
{
	GENERATED_BODY()


	//ACharacterPlayerState();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void SetSFXVolume();

	// ** Charcter selction per player ** 
	UPROPERTY(Replicated)
	int PlayerIndex = -1;


	// ** The Enum of the character selected **
	UPROPERTY(Replicated)
	ECharacter charEnum;


	// ** The index of the selected character **
	UPROPERTY(Replicated)
	int NumOfSelected = -1;


	// ** Setting the default player not ready for each playerstate object
	UPROPERTY(Replicated)
	EPlayerReady IsPlayerReady = EPlayerReady::NotReady;


	UPROPERTY()
	UTexture2D* SelectedImage;


public:
	
	// Getters and setters for the players states information with in relation to character select
	void SetPlayerIndex(int index) { PlayerIndex = index; };
	int  GetPlayerIndex() { return PlayerIndex; }

	void SetPlayerCharacterEnum(ECharacter num) { charEnum = num; };
	ECharacter GetPlayerCharacterenum() { return charEnum; }

	void SetNumOfSelectedIndex(int index) { NumOfSelected = index; };
	int GetNumOfSelectedIndex() { return NumOfSelected; }

	void SetIsPlayerReady(EPlayerReady status) { IsPlayerReady = status; };
	EPlayerReady GetIsPlayerReady() { return IsPlayerReady; }

	void Trigger_ChangeSelectImg();

	//UFUNCTION(Server, WithValidation, Reliable)
	//void ChangeSelectImg();

//New Attempt:
	void Trigger_SelectCharacter(int32 cNum);

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SetAndGetSelected(int32 cNum);

	void Trigger_SetPlayerReady(EPlayerReady IsReady);

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SetAndGetPlayerReady(EPlayerReady IsReady);

	UFUNCTION(Server, WithValidation, Reliable)
	void Trigger_CheckReadyServerTravel();

	UFUNCTION(Server, WithValidation, Reliable)
	void SetBannerReady();

};
