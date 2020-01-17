// Copywrite Shapeshifter 2019

#include "AIFighterController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine.h"
#include "FighterBase.h"

AAIFighterController::AAIFighterController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bSetControlRotationFromPawnOrientation = true;
}

void AAIFighterController::BeginPlay()
{
	Super::BeginPlay();

	Pawn = Cast<AFighterBase>(GetPawn());

	AIState = EAIState::LookingForTarget;
}

void AAIFighterController::LookingForTargetState()
{
	AActor* Target = nullptr;
	TArray<AActor*> targetList;
	while(Target == nullptr)
	{
		UGameplayStatics::GetAllActorsOfClass(this, AFighterBase::StaticClass(), targetList);
		
		int32 index = -targetList.Find(this->GetPawn());
		targetList.RemoveAt(index, 1, true);
		targetList.Remove(this->GetPawn());
		
		Target = targetList[FMath::RandRange(0,targetList.Num()-1)];
		TargetTest = targetList[FMath::RandRange(0, targetList.Num() - 1)];
	}
	AIState = EAIState::MovingToTarget;
}

void AAIFighterController::MovingToTargetState(AActor* newTarget)
{
	MoveToActor(newTarget, 10.0f);
	SetFocus(newTarget);

	float dist = FVector::Dist(Pawn->GetActorLocation(), TargetTest->GetActorLocation());

	if (dist < distToCharCheck)
	{
		AIState = EAIState::Attacking;
	}
}

void AAIFighterController::AttackState()
{
	if (isAttacking == false)
	{
		int32 randNum = FMath::RandRange(0, 50);
		switch (randNum)
		{
		case 0:
			ComboOne();
			break;
		case 1:
			ComboTwo();
			break;
		case 2:
			ComboThree();
		default:
			break;
		}
	}
}

void AAIFighterController::ComboOne()
{
	isAttacking = true;
	
	ResetAttackNum();
	//StopMovement();

	while (isAttacking == true)
	{
		if (attackNum == 0)
		{
			//FString thingyToPrint = "attack One";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
			
			bool hasAttacked = false;
			if (hasAttacked == false)
			{
				Pawn->LightAtk();
				hasAttacked = true;
			}
			attackNum = 1;
		}
		else if (attackNum == 1)
		{
			//FString thingyToPrint = "attack two";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
			
			bool hasAttacked = false;
			if (hasAttacked == false)
			{
				Pawn->LightAtk();
				hasAttacked = true;
			}
			isAttacking = false;
		}
		else
		{
			AFighterBase* targetBase = Cast<AFighterBase>(TargetTest->GetClass());
			if (targetBase->GetHealth() <= 0 || targetBase == nullptr)
			{
				AIState = EAIState::LookingForTarget;
			}
			else
			{
				AIState = EAIState::MovingToTarget;
			}
		}
	}
}

void AAIFighterController::ComboTwo()
{
	isAttacking = true;

	ResetAttackNum();
	//StopMovement();

	while (isAttacking == true)
	{
		if (attackNum == 0)
		{
			//FString thingyToPrint = "attack One";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));

			bool hasAttacked = false;
			if (hasAttacked == false)
			{
				Pawn->LightAtk();
				hasAttacked = true;
			}
			attackNum = 1;
		}
		else if (attackNum == 1)
		{
			//FString thingyToPrint = "attack two";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));

			bool hasAttacked = false;
			if (hasAttacked == false)
			{
				Pawn->LightAtk();
				hasAttacked = true;
			}
			isAttacking = false;
		}
		else
		{
			AFighterBase* targetBase = Cast<AFighterBase>(TargetTest->GetClass());
			if (targetBase->GetHealth() <= 0 || targetBase == nullptr)
			{
				AIState = EAIState::LookingForTarget;
			}
			else
			{
				AIState = EAIState::MovingToTarget;
			}
		}
	}
}

void AAIFighterController::ComboThree()
{
	isAttacking = true;

	ResetAttackNum();
	//StopMovement();

	while (isAttacking == true)
	{
		if (attackNum == 0)
		{
			//FString thingyToPrint = "attack One";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));

			bool hasAttacked = false;
			if (hasAttacked == false)
			{
				Pawn->SpecAtk();
				hasAttacked = true;
			}
			attackNum = 1;
		}
		else if (attackNum == 1)
		{
			//FString thingyToPrint = "attack two";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));

			bool hasAttacked = false;
			if (hasAttacked == false)
			{
				Pawn->LightAtk();
				hasAttacked = true;
			}
			isAttacking = false;
		}
		else
		{
			AFighterBase* targetBase = Cast<AFighterBase>(TargetTest->GetClass());
			if (targetBase->GetHealth() <= 0 || targetBase == nullptr)
			{
				AIState = EAIState::LookingForTarget;
			}
			else
			{
				AIState = EAIState::MovingToTarget;
			}
		}
	}
}

void AAIFighterController::ResetAttackNum()
{
	if (attackNum != 0)
	{
		attackNum = 0;
	}
}

void AAIFighterController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
}

void AAIFighterController::UnPossess()
{
	Super::UnPossess();
}

void AAIFighterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (AIState)
	{
	case EAIState::LookingForTarget:
		LookingForTargetState();
		break;
	case EAIState::MovingToTarget:
		MovingToTargetState(TargetTest);
		break;
	case EAIState::Attacking:
		AttackState();
		break;
	default:
		break;
	}

	if (TargetTest != nullptr && TargetTest != Pawn)
	{
		SetFocus(TargetTest);
		
		float y1 = TargetTest->GetActorLocation().Y;
		float y2 = this->GetPawn()->GetActorLocation().Y;

		if (y1 > y2)
		{
			Pawn->SetIsFacingRight(true);
		}
		else
		{
			Pawn->SetIsFacingRight(false);
		}
	}
}

