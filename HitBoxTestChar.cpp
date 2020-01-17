// Copywrite Shapeshifter 2019

#include "HitBoxTestChar.h"
#include "FighterBase.h"
#include "UnrealNetwork.h"
#include "Engine.h"
#include "TimerManager.h"
#include "FighterMovementComponent.h"
#include "HitboxComponent.h"
#include "Engine/EngineTypes.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"

AHitBoxTestChar::AHitBoxTestChar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	//MakeHitboxesVisible();

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
	GetCapsuleComponent()->CanEditSimulatePhysics();
}

void AHitBoxTestChar::BeginPlay()
{
	Super::BeginPlay();

}


void AHitBoxTestChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

