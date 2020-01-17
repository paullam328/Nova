// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UnrealNetwork.h"
#include "LeaderboardData.h"
#include "LeaderboardPawn.generated.h"

class ULeaderboardWidget;
class UStageSwitchWipeWidget;

UCLASS()
class NOVAFINAL_API ALeaderboardPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALeaderboardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
	TSubclassOf<ULeaderboardWidget> LeaderboardWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
	TSubclassOf<UStageSwitchWipeWidget> StageSwitchWipeWidgetClass;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetLeaderboardText(const TArray<FLeaderboardData>& data);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ULeaderboardWidget* LeaderboardWidget;

	UStageSwitchWipeWidget* StageSwitchWipeWidget;

	UFUNCTION(Server, WithValidation, Reliable)
	void RetrieveAndRenderMatchInfo();

//Kick me out!:
	void Trigger_KickMeOut();

	UFUNCTION(Server, WithValidation, Reliable)
	void KickMeOut();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void InitializeTransitionWidget();

	void SetMusicVolume();
};
