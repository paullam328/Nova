// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Animation/SkeletalMeshActor.h"
#include "UnrealNetwork.h"
#include "EPlayerReady.h"
#include "CharacterSelectPawn.generated.h"

class UCharacterSelectWidget;
//
UENUM()
enum class ECharacters : uint8
{
	//Index + 1;
	Invalid = 0,
	Demark,
	Maskium,
};


UCLASS()
class NOVAFINAL_API ACharacterSelectPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACharacterSelectPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, WithValidation, Reliable)
	void SetCharEnumSelected();

	UPROPERTY(Replicated)
	ECharacters charEnum;

	UPROPERTY(Replicated)
	int PlayerIndex = -1; //this indicates which player you are

	UPROPERTY(Replicated)
	int num = 0;

	int setNum = -2;


	UPROPERTY(Replicated)
	bool IsIndexSet = false;

	UCharacterSelectWidget* CharacterSelectWidget;

//Setting selected characters:---------------------------------------
	void Trigger_ChangeSelectImg();

	UFUNCTION(Server, WithValidation, Reliable)
	void ChangeSelectImg();

	//What if, the values are being grabbed from the game state and passed to the pawn
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

	//UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetMyPlayerName();

//Setting selected characters:---------------------------------------
	UFUNCTION(Server, WithValidation, Reliable)
	void UpdateSelectArray();

//Checking whether players are ready:---------------------------------
	UFUNCTION(Server, WithValidation, Reliable)
	void UpdatePlayersReady();


private:

	TArray<ASkeletalMeshActor*> allModels;

	void FillArray();

};
