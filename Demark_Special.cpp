// Copywrite Shapeshifter 2019

#include "Demark_Special.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FighterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "Particles/ParticleSystemComponent.h"

#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"
#include "AkComponent.h"
#include "BarrierBase.h"
#include "NetworkingGameInstance.h"

// Sets default values
ADemark_Special::ADemark_Special(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	RootComponent = Hitbox;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->bReplicates = true;
	this->bReplicateMovement = true;


}

void ADemark_Special::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADemark_Special, Movement);
	DOREPLIFETIME(ADemark_Special, Hitbox);
}

// Called when the game starts or when spawned
void ADemark_Special::BeginPlay()
{
	Super::BeginPlay();
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ADemark_Special::OnOverlap);
	
	UGameplayStatics::SpawnEmitterAttached(ShotSystem, RootComponent, FName(""), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	
}

// Called every frame
void ADemark_Special::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
bool ADemark_Special::OnOverlap_Validate(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	return true;
}
void ADemark_Special::OnOverlap_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	this->Destroy();
	
	
	AFighterBase* Fighter = Cast<AFighterBase>(OtherActor);
	if (Fighter == nullptr || Fighter == this->GetOwner()) return;	

	Fighter->DoHitChar(OverlappedComp, OtherComp, this, EHitPower::SPECIAL, 10.f, 1.2f, FVector_NetQuantize(0.0f, 1500.f, 1500.f));
	Fighter->SpawnHitSplat(OtherComp, EHitPower::SPECIAL);
	//PlayHitSound(Fighter);
		
}

bool ADemark_Special::PlaySpecialHitSound_Validate(AActor* act)
{
	return true;
}
void ADemark_Special::PlaySpecialHitSound_Implementation(AActor* act)
{
	//UAkGameplayStatics::PostEvent(HitSound, FB, false, "");

	UAkComponent * AkComp = nullptr;
	if (Cast<AFighterBase>(act))
	{
		AkComp = UAkGameplayStatics::SpawnAkComponentAtLocation(GetWorld(), FighterHitSound, nullptr, act->GetActorLocation(), FRotator(0, 0, 0), false, "", "", true);
	}
	else if (Cast<ABarrierBase>(act))
	{
		AkComp = UAkGameplayStatics::SpawnAkComponentAtLocation(GetWorld(), BarrierHitSound, nullptr, act->GetActorLocation(), FRotator(0, 0, 0), false, "", "", true);
	}

	if (AkComp != nullptr)
	{
		//Find the volume slider:
		UNetworkingGameInstance* GI = Cast<UNetworkingGameInstance>(GetWorld()->GetGameInstance());
		if (GI != nullptr)
		{
			AkComp->SetOutputBusVolume(GI->SFXSliderVal);
			AkComp->PostAssociatedAkEvent();
		}
	}
}