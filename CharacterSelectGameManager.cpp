// Copywrite Shapeshifter 2019

#include "CharacterSelectGameManager.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ACharacterSelectGameManager::ACharacterSelectGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterSelectGameManager::BeginPlay()
{
	Super::BeginPlay();
	/*
	UWorld* World = GetWorld();
	if (World)
	{
		if (CharacterSelectWidget == nullptr && CharacterSelectWidgetClass != nullptr)
		{
			FillArray();

			CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(World, CharacterSelectWidgetClass);
			CharacterSelectWidget->AddToViewport();
			//UGameplayStatics::GetPlayerController(World, 0)->SetInputMode();
			APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
			UWidgetBlueprintLibrary::SetInputMode_UIOnly(PC, CharacterSelectWidget->Character1Button, false);

			TArray<AActor*> arrOfCams;
			UGameplayStatics::GetAllActorsOfClass(World, ACameraActor::StaticClass(), arrOfCams);
			ACameraActor* cam = Cast<ACameraActor>(arrOfCams[0]);
			PC->SetViewTargetWithBlend(cam, 0.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);



		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Widget is detached from CharacterSelectGameManager_BP.  Please re-attach them!")));
		}
	}*/
}

// Called every frame
void ACharacterSelectGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (CharacterSelectWidget != nullptr)
	//{
	//	num = CharacterSelectWidget->CharNum;

	//	for (int i = 0; i < allModels.Num(); i++)
	//	{
	//		if (i == num)
	//		{
	//			allModels[i]->GetSkeletalMeshComponent()->SetVisibility(true);
	//		}
	//		else
	//		{
	//			allModels[i]->GetSkeletalMeshComponent()->SetVisibility(false);
	//		}
	//	}
	//}

}

//
//void ACharacterSelectGameManager::FillArray()
//{
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		FString idle = "Idle";
//		TArray<AActor*> idles;
//		UGameplayStatics::GetAllActorsWithTag(World, FName(*idle), idles);
//
//		FString maskium = "Maskium";
//		TArray<AActor*> masks;
//		UGameplayStatics::GetAllActorsWithTag(World, FName(*maskium), masks);
//
//		allModels.Add(Cast<ASkeletalMeshActor>(idles[0]));
//		allModels.Add(Cast<ASkeletalMeshActor>(masks[0]));
//	}
//}