// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "LevelMusicManager.generated.h"

class AFighterController;
class UAkAudioEvent;
class UAkComponent;

UCLASS()
class NOVAFINAL_API ALevelMusicManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelMusicManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//AK Events:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent * Stage1Event;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent * Stage2Event;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent * MainMenuEvent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void AdjustMusicSlider(int stage, bool isFighterInitialization);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void PlayMusicBasedOnStage(float vol, int stage);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InitializeLevelMusicManager();

	void MainMenu_AdjustMusSlider(float MusSliderVal);

	//UPROPERTY(Replicated)
	bool isMusicPlaying = false;


private:
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Client_PostMusic(bool IsMainMenu, bool IsStage);

	UPROPERTY(Replicated)
	bool isEventPosted = false;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetIsEventPosted();

	void PostEvent();
};
