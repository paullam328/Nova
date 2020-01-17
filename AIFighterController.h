// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FighterBase.h"
#include "AIFighterController.generated.h"


class AFighterBase;
/**
 * 
 */


UENUM(BlueprintType)
enum class EAIState : uint8
{
	LookingForTarget,
	MovingToTarget,
	Attacking
};


UCLASS()
class NOVAFINAL_API AAIFighterController : public AAIController
{
	GENERATED_BODY()

	AAIFighterController(const FObjectInitializer& ObjectInitializer);

private:
	virtual void BeginPlay() override;

	TArray<AActor*> targetList;

	AActor* TargetTest;

	AFighterBase* Pawn;

	UFUNCTION()
		void LookingForTargetState();
	UFUNCTION()
		void MovingToTargetState(AActor* newTarget);
	UFUNCTION()
		void AttackState();
	UFUNCTION()
		void ComboOne();
	UFUNCTION()
		void ComboTwo();
	UFUNCTION()
		void ComboThree();

	UFUNCTION()
		void ResetAttackNum();

	

public:
	virtual void Possess(APawn* InPawn) override;

	virtual void UnPossess() override;

	virtual void Tick(float DeltaSeconds) override;

	int32 attackNum = 0;

	UPROPERTY(EditAnywhere)
		float distToCharCheck = 100.0f;

	bool isAttacking = false;

	EAIState AIState;
	
};
