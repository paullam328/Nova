// Copywrite Shapeshifter 2019

#include "LeaderboardGameMode.h"
#include "Engine.h"
#include "LeaderboardPawn.h"

ALeaderboardGameMode::ALeaderboardGameMode()
{
	/*bActorSeamlessTraveled = false;

	bUseSeamlessTravel = false;

	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		UNetDriver* Driver = World->GetNetDriver();


		if (Driver != nullptr)
		{
			Driver->bClampListenServerTickRate = true;
		}
	}*/
}


void ALeaderboardGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerControllerList.Add(NewPlayer);

	UWorld* World = GetWorld();


	FActorSpawnParameters SpawnParams;
	FRotator rotator;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	if (World)
	{
		
		int numOfPlayers = PlayerControllerList.Num() - 1;

		FString Position = FString::FromInt(numOfPlayers);

		APawn* Player = World->SpawnActor<ALeaderboardPawn>(characterToSpawn, FindPlayerStart(NewPlayer, Position)->GetActorTransform(), SpawnParams);
			
		NewPlayer->Possess(Player);


	}
}

void ALeaderboardGameMode::HandleSeamlessTravelPlayer(AController *& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	APlayerController* NewPlayer = Cast<APlayerController>(C);

	PlayerControllerList.Add(NewPlayer);

	UWorld* World = GetWorld();


	FActorSpawnParameters SpawnParams;
	FRotator rotator;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	if (World)
	{

		int numOfPlayers = PlayerControllerList.Num() - 1;

		FString Position = FString::FromInt(numOfPlayers);

		APawn* Player = World->SpawnActor<ALeaderboardPawn>(characterToSpawn, FindPlayerStart(NewPlayer, Position)->GetActorTransform(), SpawnParams);

		NewPlayer->Possess(Player);


	}

}