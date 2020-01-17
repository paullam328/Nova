// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "gamePlayUserHUD.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API AgamePlayUserHUD : public AHUD
{
	GENERATED_BODY()

public:

	AgamePlayUserHUD();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UgamePlayUserWidget> WidgetClass;

	class UgamePlayUserWidget* widget;     
	void Pause();
	bool isPaused = false;
	
private:


	void unPause();
};
