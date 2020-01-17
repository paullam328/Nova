// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "FighterBase.h"
#include "GamePlayCamera.generated.h"

class UCameraComponent;

UCLASS()
class NOVAFINAL_API AGamePlayCamera : public AActor
{
	GENERATED_BODY()

public:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* cameraComponent;

	
	
public:	
	// Sets default values for this actor's properties
	AGamePlayCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GameStartCamSetup(AActor* p1, AActor* p2);

};
