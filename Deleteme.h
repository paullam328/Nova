// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Deleteme.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API ADeleteme : public APlayerController
{
	GENERATED_BODY()

public:

	/** Handles a key press */
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;

};
