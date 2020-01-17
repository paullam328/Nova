// Fill out your copyright notice in the Description page of Project Settings.

#include "StageFloorsGenerator.h"
#include "Engine.h"


// Sets default values
AStageFloorsGenerator::AStageFloorsGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	offsetZ = FVector(0,0,400);
}

// Called when the game starts or when spawned
void AStageFloorsGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateFloors();
}

// Called every frame
void AStageFloorsGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStageFloorsGenerator::GenerateFloors()
{
	if (numOfPlayers >= 2)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(RandomFloorGeneratorClass, GetActorLocation() + offsetZ, GetActorRotation(), SpawnParams);
	}
	if (numOfPlayers >= 4)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(RandomFloorGeneratorClass, GetActorLocation() + offsetZ * 2, GetActorRotation(), SpawnParams);
	}
	if (numOfPlayers >= 7)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(RandomFloorGeneratorClass, GetActorLocation() + offsetZ * 3, GetActorRotation(), SpawnParams);
	}
	if (numOfPlayers <= 1)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Warning: Number of player per stage at start should be larger than 2 to Spawn the Platforms!")));
	}

}

