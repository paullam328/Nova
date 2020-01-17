// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Animation/SkeletalMeshActor.h"
#include "CharacterSelectGameManager.generated.h"



UCLASS()
class NOVAFINAL_API ACharacterSelectGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSelectGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
		TSubclassOf<UCharacterSelectWidget> CharacterSelectWidgetClass;

	UCharacterSelectWidget* CharacterSelectWidget;

private:
	int num = 0;
	TArray<ASkeletalMeshActor*> allModels;

	void FillArray();
	*/

};
