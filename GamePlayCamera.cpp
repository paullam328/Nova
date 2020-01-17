// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayCamera.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FighterBase.h"
#include "Engine.h"

// Sets default values
AGamePlayCamera::AGamePlayCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComponent"));
	RootComponent = cameraComponent;
}

// Called when the game starts or when spawned
void AGamePlayCamera::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFighterBase::StaticClass(), FoundActors);

	//FString thingyToPrint = FString::FromInt(FoundActors.Num());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));

	if (FoundActors.Num() == 2)
	{
		GameStartCamSetup(FoundActors[0], FoundActors[1]);
		/*FString thingyToPrint = FString::SanitizeFloat(FoundActors[0]->GetActorLocation().X);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));*/
	}

}

// Called every frame
void AGamePlayCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGamePlayCamera::GameStartCamSetup(AActor* p1, AActor* p2)
{
	// change to X axis in the new stages
	float p1Y = p1->GetActorLocation().Y;
	float p2Y = p2->GetActorLocation().Y;
	FString thingyToPrint = FString::SanitizeFloat(p1Y);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
	FVector newCamPos = FVector(this->GetActorLocation().X, (p1Y + p2Y) / 2, this->GetActorLocation().Z);

	this->SetActorLocation(newCamPos, false);
}



