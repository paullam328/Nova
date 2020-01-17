// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "NovaGameSession.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API ANovaGameSession : public AGameSession
{
	GENERATED_BODY()


	ANovaGameSession();

public:



	UFUNCTION(BlueprintCallable, Category = "Network")
	virtual void RegisterServer() override;


	/*virtual bool KickPlayer(APlayerController* KickedPlayer, const FText& KickReason) override;*/
	
};
