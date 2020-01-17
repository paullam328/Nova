// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "NovaGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UNovaGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()


	UNovaGameViewportClient();

public:
	virtual TOptional<bool> QueryShowFocus(const EFocusCause InFocusCause) const override;
};
