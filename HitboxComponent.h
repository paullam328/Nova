// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HitboxComponent.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UHitboxComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
		bool GetCanDamage() const { return bCanDamage; }
	UFUNCTION()
		void SetCanDamage(bool nextActive);
private:
	UPROPERTY(Editanywhere, Replicated)
	bool bCanDamage;
	
};
