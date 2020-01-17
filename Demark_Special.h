// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Demark_Special.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UParticleSystemComponent;

class UAkAudioEvent;

class AFighterBase;

UCLASS()
class NOVAFINAL_API ADemark_Special : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADemark_Special(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Replicated)
	UProjectileMovementComponent* Movement;
	UPROPERTY(EditAnywhere , Replicated)
	UBoxComponent* Hitbox;

	UFUNCTION(Server, WithValidation, Reliable)
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	//UFUNCTION

	UPROPERTY(EditAnywhere, Replicated)
	UParticleSystem* ShotSystem;
protected:
	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable)
	void PlaySpecialHitSound(AActor* act);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * FighterHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
	UAkAudioEvent * BarrierHitSound;

};
