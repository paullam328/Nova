// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "ElevatorCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API UElevatorCameraShake : public UCameraShake
{
	GENERATED_BODY()

	UElevatorCameraShake();


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _oscillationDuration = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _oscillationBlendInTime = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _oscillationBlendOutTime = 0.05f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pitchAmplitudeMin = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pitchAmplitudeMax = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pitchFrequencyMin = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pitchFrequencyMax = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float yawAmplitudeMin = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float yawAmplitudeMax = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float yawFrequencyMin = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float yawFrequencyMax = 35.0f;
};
