// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "BarrierBase.generated.h"


class UBoxComponent;
class UAkAudioEvent;
class AFighterBase;

UCLASS()
class NOVAFINAL_API ABarrierBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarrierBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region CollisionFunctions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherFighter, UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& sweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector_NetQuantize HitBounceVec = FVector_NetQuantize(0.0f, 2000.0f, 600.0f);
#pragma endregion

	UFUNCTION(BlueprintCallable)
	void Wwise_OverlapBarrierTrigger(AActor* Fighter);

};
