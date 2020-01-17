// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UnrealNetwork.h"
#include "FighterSpectator.generated.h"

class UCharacterCameraComp;

UCLASS()
class NOVAFINAL_API AFighterSpectator : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFighterSpectator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
	TSubclassOf<UUserWidget> SpectatorScreenClass;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InitializeSpectatorScreen();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterCameraComp* CameraComp;
private:
	UUserWidget* SpectatorScreen;

};
