// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "UnrealNetwork.h"
#include "CharacterCameraComp.generated.h"



class UCameraShake;
/**
 * 
 */
UENUM(BlueprintType)
enum class ECameraStateEnum : uint8
{
	GameplayState,
	ElevatorState
};

UCLASS()
class NOVAFINAL_API UCharacterCameraComp : public UCameraComponent
{
	GENERATED_BODY()
	

public:
	UCharacterCameraComp();

	void CameraMove(AActor* p1, AActor* p2);
	void CameraZoom(AActor* p1, AActor* p2);
	AActor* FindLeftMostPlayer();
	AActor* FindRightMostPlayer();

	UPROPERTY(EditAnywhere)
	float minZoomClamp = -1500;
	UPROPERTY(EditAnywhere)
	float maxZoomClamp = -1300;

	TArray<AActor*> FoundActors;
	TMap<float, AActor*> distMap;

	FVector NewLocation(TArray<AActor*> allActors);

	FVector ElevatorLocation(TArray<AActor*> allActors);

	UPROPERTY(Replicated)
	ECameraStateEnum CameraState;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> MyShake;

	UFUNCTION()
	void SetCamState(ECameraStateEnum newState) { CameraState = newState; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool useNewCamera = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, WithValidation, Reliable)
	void ToggleCameraShake();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void DoCameraShake();

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void StopCameraShake();

	UPROPERTY(Replicated)
	bool HasCameraShakePlayed;

public:
	virtual void TickComponent(float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction * ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		float zOffSet = 10.0f;

};
