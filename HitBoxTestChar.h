// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "FighterBase.h"
#include "HitBoxTestChar.generated.h"

/**
 * 
 */
class UHitboxComponent;

UCLASS()
class NOVAFINAL_API AHitBoxTestChar : public ACharacter
{
	GENERATED_BODY()

public:
	AHitBoxTestChar(const FObjectInitializer& ObjectInitializer);


	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
