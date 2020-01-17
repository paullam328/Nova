// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomFloorGenerator.h"
#include "Engine.h"

// Sets default values
ARandomFloorGenerator::ARandomFloorGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARandomFloorGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateRandomFloor();
}

// Called every frame
void ARandomFloorGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARandomFloorGenerator::GenerateRandomFloor()
{
	if (randomFloors.Num() < 1) { return; }
	int rand = FMath::RandRange(0, randomFloors.Num() - 1);
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AActor>(randomFloors[rand],GetActorLocation(),GetActorRotation(), SpawnParams);
}



