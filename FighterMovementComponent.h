// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FighterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UFighterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit, ETeleportType Teleport);
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;
	virtual void JumpOff(AActor* MovementBaseActor) override;
public:

	bool bIsFacingRight;
};
