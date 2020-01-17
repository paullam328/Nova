// Copywrite Shapeshifter 2019

#include "CharacterCameraComp.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "FighterBase.h"
#include "FighterController.h"
#include "Camera/PlayerCameraManager.h"

UCharacterCameraComp::UCharacterCameraComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	bReplicates = true;
	SetIsReplicated(true);
}

FVector UCharacterCameraComp::NewLocation(TArray<AActor*> allActors)
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	for(int i=0; i<allActors.Num(); ++i)
	{
		FVector actorLocation;
		if (useNewCamera == true)
		{
			actorLocation = UGameplayStatics::GetActorArrayAverageLocation(allActors);
		}
		else
		{
			actorLocation = allActors[i]->GetActorLocation();
		}
		 
		if (actorLocation.Z < minX)
		{
			minX = actorLocation.Z;
		}
		if (actorLocation.Y < minY)
		{
			minY = actorLocation.Y;
		}
		if (actorLocation.Z > maxX)
		{
			maxX = actorLocation.Z;
		}
		if (actorLocation.Y > maxY)
		{
			maxY = actorLocation.Y;
		}
	}

	float xVal = -FVector::Dist(FVector(minX, minY, 0.0f), FVector(maxX, maxY, 0.0f));

	FVector returnVal = FVector(FMath::Clamp(xVal, minZoomClamp, maxZoomClamp), (minY + maxY)*0.5f, (minX + maxX)* 0.5f + zOffSet);

	return returnVal;

	/*
	//float xVal = -FVector::Dist(FVector(minX, minY, 0.0f), FVector(maxX, maxY, 0.0f));
	float xVal = UGameplayStatics::GetActorArrayAverageLocation(allActors).X;

	FVector returnVal = FVector(FMath::Clamp(xVal, minZoomClamp, maxZoomClamp), UGameplayStatics::GetActorArrayAverageLocation(allActors).Y, UGameplayStatics::GetActorArrayAverageLocation(allActors).Z + zOffSet);

	return returnVal;*/
}

FVector UCharacterCameraComp::ElevatorLocation(TArray<AActor*> allActors)
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	for (int i = 0; i < allActors.Num(); ++i)
	{
		FVector actorLocation = allActors[i]->GetActorLocation();
		if (actorLocation.Z < minX)
		{
			minX = actorLocation.Z;
		}
		if (actorLocation.Y < minY)
		{
			minY = actorLocation.Y;
		}
		if (actorLocation.Z > maxX)
		{
			maxX = actorLocation.Z;
		}
		if (actorLocation.Y > maxY)
		{
			maxY = actorLocation.Y;
		}
	}

	float xVal = -FVector::Dist(FVector(minX, minY, 0.0f), FVector(maxX, maxY, 0.0f));

	FVector returnVal = FVector(FMath::Clamp(xVal, minZoomClamp, maxZoomClamp), (minY + maxY)*0.5f, (minX + maxX)*0.5f + zOffSet);

	return returnVal;
}

void UCharacterCameraComp::BeginPlay()
{
	Super::BeginPlay();
	//CameraState = ECameraStateEnum::GameplayState;
}

bool UCharacterCameraComp::DoCameraShake_Validate()
{
	return true;
}


void UCharacterCameraComp::DoCameraShake_Implementation()
{
	if (!HasCameraShakePlayed)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 1.0f);
		HasCameraShakePlayed = true;
	}
}

bool UCharacterCameraComp::StopCameraShake_Validate()
{
	return true;
}


void UCharacterCameraComp::StopCameraShake_Implementation()
{
	if (HasCameraShakePlayed)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StopAllCameraShakes(true);
		HasCameraShakePlayed = false;
	}
}


bool UCharacterCameraComp::ToggleCameraShake_Validate()
{
	return true;
}


void UCharacterCameraComp::ToggleCameraShake_Implementation()
{
	//if (Role == ROLE_Authority)
	//{
		switch (CameraState)
		{
		case ECameraStateEnum::GameplayState:
			StopCameraShake();
			break;
		case ECameraStateEnum::ElevatorState:
			DoCameraShake();
			break;
		}
	//}
}


void UCharacterCameraComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFighterBase::StaticClass(), FoundActors);

	switch (CameraState)
	{
	case ECameraStateEnum::GameplayState:
		this->SetWorldLocation(NewLocation(FoundActors));
		break;
	case ECameraStateEnum::ElevatorState:
		this->SetWorldLocation(ElevatorLocation(FoundActors));
		break;
	}

	ToggleCameraShake();

}

void UCharacterCameraComp::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterCameraComp, HasCameraShakePlayed);
	DOREPLIFETIME(UCharacterCameraComp, CameraState);
}








////// VVV IGNORE VVV ////////
void UCharacterCameraComp::CameraMove(AActor * p1, AActor * p2)
{
	float p1Y = p1->GetActorLocation().Y; // change to x if rotate the map
	float p2Y = p2->GetActorLocation().Y;
	/*FString thingyToPrint = FString::SanitizeFloat(p1Y);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));*/
	FVector newCamPos = FVector(this->GetComponentLocation().X, (p1Y + p2Y) / 2, this->GetComponentLocation().Z);

	this->SetWorldLocation(newCamPos, true);
}

void UCharacterCameraComp::CameraZoom(AActor * p1, AActor * p2)
{
	float p1Y = p1->GetActorLocation().Y; // change to x if rotate the map
	float p2Y = p2->GetActorLocation().Y;
	float zoom;
	if ((p2Y - p1Y) * 0.1 < 0.0f)
	{
		zoom = (p2Y - p1Y) * -0.1;
	}
	else
	{
		zoom = (p2Y - p1Y) * 0.1;
	}
	float FOV = FMath::Clamp(zoom, minZoomClamp, maxZoomClamp);
	this->SetFieldOfView(FOV);
}

AActor* UCharacterCameraComp::FindLeftMostPlayer()
{
	AActor* leftMostPlayer = nullptr;
	AActor* temp = nullptr;

	/*for (size_t i = 0; i < FoundActors.Num(); i++)
	{
		float dist = FVector::Dist(FoundActors[i]->GetActorLocation(), FVector(0.0f, 0.0f, 0.0f));
		distMap.Add(dist, FoundActors[i]);
	}
	TArray<float> distances;
	distMap.GenerateKeyArray(distances);
	Algo::Sort(distances);
*/
	

	/*for (size_t i = 0; i < distances.Num()-1; i++)
	{
		FString thingyToPrint = FString::SanitizeFloat(distances[i]);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));
	}*/
	
	for (size_t i = 0; i < FoundActors.Num() -1; i++)
	{
		if (FoundActors[i] != nullptr && FoundActors[i + 1] != nullptr)
		{
			if (temp == nullptr)
			{
				temp = FoundActors[0];
			}
			if (temp->GetActorLocation().Y < FoundActors[i + 1]->GetActorLocation().Y)// less then = left side
			{
				temp = FoundActors[i];
			}
			else
			{
				temp = FoundActors[i + 1];
			}
		}
	}
	/*FString thingyToPrint = this->GetOwner()->GetName() + " Thinks " + temp->GetName() + " is far left";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));*/

	leftMostPlayer = temp;

	//FString thingyToPrint = this->GetOwner()->GetName() + " Thinks " + leftMostPlayer->GetName() + " is far left";
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));*/

	return leftMostPlayer;
}

AActor* UCharacterCameraComp::FindRightMostPlayer()
{
	AActor* rightMostPlayer;
	AActor* temp = nullptr;

	for (size_t i = 0; i < FoundActors.Num() -1; i++)
	{
		if (FoundActors[i] != nullptr && FoundActors[i + 1] != nullptr)
		{
			if (temp == nullptr)
			{
				temp = FoundActors[0];
			}
			if (temp->GetActorLocation().Y > FoundActors[i + 1]->GetActorLocation().Y)// greater then = right side
			{
				temp = FoundActors[i];
			}
			else
			{
				temp = FoundActors[i + 1];
			}
		}
	}

	rightMostPlayer = temp;

	return rightMostPlayer;
}


