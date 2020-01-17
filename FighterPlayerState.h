// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UnrealNetwork.h"

#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"

#include "FighterPlayerState.generated.h"

/**
 * 
 */
class ACharacterSelectGameManager;
class AFighterBase;

UCLASS()
class NOVAFINAL_API AFighterPlayerState : public APlayerState
{
	GENERATED_BODY()

		 
public: 

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	virtual void UpdatePing(float InPing) override;
	
	virtual void CalculatePing(float NewPing);

	UPROPERTY(EditAnywhere, Category="Manager")
	TSubclassOf<ACharacterSelectGameManager> CharacterSelectGameManagerClass;

	ACharacterSelectGameManager* CharacterSelectGameManager;

	UPROPERTY(Replicated)
	bool isSelectedCharSet = false;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int PlayerIndex = 0;

	UPROPERTY(Replicated)
	AFighterBase* PawnToPossess;

	//For edge cases:
	UPROPERTY(Replicated)
	bool IsEventPosted = false;

	UPROPERTY()
	TArray<int32> allCharacterIndexValues;

	UPROPERTY()
	TArray<FString> Names = { "XxShortxX", "XxCaringxX", "Chunky", "bob", "name123", "Mista Kidd", "Diamond Cheese", "Dr. Jumping Banana", "Prax Deathbringer", "Unstable Lurker", "Doc Ruff Bubba", "Fiery Pusher", "Loco Lion", "KinG Cr1msoN TAnk", "President Unstable Noob", "Silly Panda", "Peri Albion", "Fl4minG OWnEr", "Queen Killer Dollah", "Empress Bouncing Cookie", "Kor Lightbringer", "Queen Chilled Pwner", "Mr Popping Banana", "Agent Nog Starkiller", "Commander Parachuting Shanker", "SalTy NInja", "Scorpius Tenebris", "Lil Scooter", "Cheatin Kandy", "Odona Aegero", "Dr. TommyGun Reaper", "Darth Zee Vura", "Mr Hopping Bunny", "General Hidden Sprayer", "Pirate Ghost Cyndi", "Kala Storm", "Rampant Fighter", "Snowy Candy", "Little Juice", "Princess Jetpack Dude", "Peri Organa", "Crispy Barber", "Wobbly Chuckles", "The Dollaz", "Swif7 TeAm Killer", "Nori Lunis", "Tiny Ninja", "Toxic Defuser", "Jimmy Muscles", "Sir Friendly Bean", "Fat TwoShoes", "Darlin Holla", "Darth Jumping Jelly", "Krag Baltar", "Fenna Vader", "Captain Hugging Bubble", "Mara Odis", "Legendary Hopper", "Wobbly Bubble", "Old Mary Jane", "FrantIc LUrK3r", "RadioactiveMage", "SupersonicFalcon", "TurboAnimal", "SolarKnight", "SolidBazooka" };

	UPROPERTY(Replicated)
	int32 nameIndex;

	UFUNCTION(Server, WithValidation, Reliable)
	void SetPlayersNames();

//Wwise stuff:

	UFUNCTION(Server, WithValidation, Reliable)
	void Trigger_InstantiateStageSwitchWipeWidget();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void AdjustVolumeSlider();

};
