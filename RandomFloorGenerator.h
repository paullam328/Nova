// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomFloorGenerator.generated.h"

UCLASS()
class NOVAFINAL_API ARandomFloorGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomFloorGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Floors")
		TArray<TSubclassOf<AActor>> randomFloors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void GenerateRandomFloor();

};
