// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "HttpActor.generated.h"


USTRUCT()
struct FPlayersInput
{
	GENERATED_USTRUCT_BODY()

	/*
	*	0 - Nothing
	*	1 - Light_Ground
	*	2 - Medium_Ground
	*	3 - Light_Air
	*	4 - Medium_Air
	*	5 - Special
	*	6 - Dodge
	*/
	int PlayerInputState;
	float CurrentDeltaTime;
	bool IsPlayerInfront;
};


USTRUCT()
struct FDataToPost
{
	GENERATED_USTRUCT_BODY();

	/*
	* The time when the Session Starts...
	*/
	float SessionTimeStart;
	
	/*
	* The time when the sessions ends..
	*/
	float SessionTimeEnd;

	/*
	* Current Player's name from Steam, or Lan which is "Player"
	*/
	FString PlayerName;
	
	/*
	* The Character selected in the game 
	*/
	FString PlayersSelectedCharacter;



	/*
	* The Players Aggression over the whole game
	*/
	int32 EndAggression;

	/*
	* Collecting Respectful data per person in a session;
	*/
	TArray<FPlayersInput> TotalPlayerInput;
	
};


UCLASS()
class NOVAFINAL_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor(const class FObjectInitializer& ObjectInitializer);

	// The Http pointer Module
	FHttpModule* Http;
	
	virtual void BeginPlay() override;

	// Get Method	
	//UFUNCTION()
	//void GetHTTPCall();
	
	// Post Method
	UFUNCTION()
	void PostHTTPCall(TArray<FDataToPost> Data);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);




public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
