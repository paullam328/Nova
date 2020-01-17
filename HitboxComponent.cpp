// Copywrite Shapeshifter 2019

#include "HitboxComponent.h"
#include "UnrealNetwork.h"
#include "Engine.h"


void UHitboxComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHitboxComponent, bCanDamage);
}

void UHitboxComponent::SetCanDamage(bool nextActive)
{
	if (nextActive)
	{
		bCanDamage = true;
		this->ShapeColor = FColor::Green;
	}
	else if (nextActive == false)
	{
		bCanDamage = false;
		this->ShapeColor = FColor::Red;
	}
}
