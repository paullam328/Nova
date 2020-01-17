// Copywrite Shapeshifter 2019

#include "ElevatorCameraShake.h"

UElevatorCameraShake::UElevatorCameraShake()
{
	OscillationDuration = _oscillationDuration;
	OscillationBlendInTime = _oscillationBlendInTime;
	OscillationBlendOutTime = _oscillationBlendOutTime;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(pitchAmplitudeMin, pitchAmplitudeMax);
	RotOscillation.Pitch.Frequency = FMath::RandRange(pitchFrequencyMin, pitchFrequencyMax);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(yawAmplitudeMin, yawAmplitudeMax);
	RotOscillation.Yaw.Frequency = FMath::RandRange(yawFrequencyMin, yawFrequencyMax);
}