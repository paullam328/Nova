// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStateEnum.h"
#include "StateMachineComponent.generated.h"

class State_Air;
class State_Ground;
class State_AirHit;
class State_GroundHit;
class State_AirAtk;
class State_GroundAtk;
enum class ECharacterStates;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOVAFINAL_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	ECharacterStates GetCurrentState() const { return CurrentState; }
	void SetCurrentState(ECharacterStates newState) { CurrentState = newState; }
	void KbEnded();
protected:
	ECharacterStates CurrentState;

private:
	State_Air* air;
	State_Ground* ground;
	State_AirHit* airHit;
	State_GroundHit* groundHit;
	State_AirAtk* airAtk;
	State_GroundAtk* groundAtk;
};
