// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemarkCharacterSelectActor.generated.h"

UCLASS()
class NOVAFINAL_API ADemarkCharacterSelectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADemarkCharacterSelectActor();

	bool isSelected = false;

	UFUNCTION()
	void SetSelected(bool newIsSlected) { isSelected = newIsSlected; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Selected", CompactNodeTitle = "Selected", Keyword = "Get Selected"))
	bool GetSelected() { return isSelected; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
