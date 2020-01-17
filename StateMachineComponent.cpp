// Copywrite Shapeshifter 2019

#include "StateMachineComponent.h"
#include "Engine.h"
#include "State_Air.h"
#include "State_AirAtk.h"
#include "State_AirHit.h"
#include "State_Ground.h"
#include "State_GroundAtk.h"
#include "State_GroundHit.h"
#include "CharacterStateEnum.h"

// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	//Pre creating the objects for use..

	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	air = new State_Air();
	airHit = new State_AirHit();
	airAtk = new State_AirAtk();
	ground = new State_Ground();
	groundHit = new State_GroundHit();
	groundAtk = new State_GroundAtk();
	CurrentState = ECharacterStates::GROUND;
	// ...
	
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateMachineComponent::KbEnded()
{
	
	if (CurrentState == ECharacterStates::HIT_AIR)
	{
		CurrentState = ECharacterStates::AIR;
	}
	else if (CurrentState == ECharacterStates::HIT_GROUND)
	{
		CurrentState = ECharacterStates::GROUND;

	}
}
