// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UnrealNetwork.h"
#include "MainMenuPlayerState.generated.h"

class UAkAudioEvent;
/**
 * 
 */
UCLASS()
class NOVAFINAL_API AMainMenuPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UFUNCTION(Server, WithValidation, Reliable)
		void GS_UnpossessPawns();

	// Called when the game starts or when spawned
		virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Possess Pawn")
		void Trigger_UnpossessPawns();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent * MainMenuEvent;
};
