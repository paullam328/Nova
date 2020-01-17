// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomFloorGenerator.h"
#include "StageFloorsGenerator.generated.h"

UCLASS()
class NOVAFINAL_API AStageFloorsGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageFloorsGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Number Of Floors")
	TSubclassOf<ARandomFloorGenerator> RandomFloorGeneratorClass;

	//Please provide the number of players, so that the number of levels per stages can be decided
	UPROPERTY(EditAnywhere, Category = "Number Of Floors")
	int numOfPlayers;

	//Spawn Z offset between each floor
	UPROPERTY(EditAnywhere, Category = "Offset between Floors")
	FVector offsetZ;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void GenerateFloors();

};
