// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StageSwitchWipeWidget.h"
#include "TransitionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API ATransitionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Transition Screen")
	TSubclassOf<UStageSwitchWipeWidget> StageSwitchWipeWidgetClass;
	

private:
	UStageSwitchWipeWidget* StageSwitchWipeWidget;
};
