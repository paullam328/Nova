// Fill out your copyright notice in the Description page of Project Settings.

#include "StageTransitionManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

// Sets default values
AStageTransitionManager::AStageTransitionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageTransitionManager::BeginPlay()
{
	Super::BeginPlay();
	TransitToStage(FName(""), FName("Stage1"));
}

// Called every frame
void AStageTransitionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UGameplayStatics::GetPlayerController(this, 0)->IsInputKeyDown(EKeys::A))
		{
			TransitToStage(FName("Stage1"), FName("Stage2"));
		}
	}
}

void AStageTransitionManager::TransitToStage(FName LevelToUnload, FName LevelToLoad)
{
	if (LevelToLoad != "")
	{
		FLatentActionInfo LoadLatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LoadLatentInfo);
	}
	if (LevelToUnload != "") 
	{
		FLatentActionInfo UnloadLatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, UnloadLatentInfo, true);
	}
}

