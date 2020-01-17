// Copywrite Shapeshifter 2019

#include "BarrierBase.h"
#include "Components/BoxComponent.h"
#include "FighterBase.h"
#include "StateMachineComponent.h"
#include "Engine.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"

// Sets default values
ABarrierBase::ABarrierBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox->SetIsReplicated(true);
}

void ABarrierBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionBox->OnComponentHit.AddDynamic(this, &ABarrierBase::OnHit);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABarrierBase::OnOverlapEnd);
}
//void ABarrierBase::GetLifetimeReplicatedProps( TArray<FLifetimeProperty> &OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}

void ABarrierBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherFighter, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& sweepResult)
{
	FVector WallFace = this->GetActorForwardVector();
	AFighterBase* Fighter = Cast<AFighterBase>(OtherFighter);
	if (Fighter == nullptr) return;
	if (Fighter->SM->GetCurrentState() == ECharacterStates::HIT_AIR || Fighter->SM->GetCurrentState() == ECharacterStates::HIT_GROUND)
	{
		HitBounceVec.Y *= WallFace.X;
		Fighter->LaunchCharacter(HitBounceVec, false, true);
		Fighter->Server_LaunchChar(HitBounceVec);
	}
	else if (Fighter->SM->GetCurrentState() == ECharacterStates::AIR || Fighter->SM->GetCurrentState() == ECharacterStates::ATK_AIR)
	{
		HitBounceVec.Y *= WallFace.X;
		Fighter->LaunchCharacter(HitBounceVec, false, true);
		Fighter->Server_LaunchChar(HitBounceVec);
	}
}

void ABarrierBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherFighter, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FVector WallFace = this->GetActorForwardVector();
	AFighterBase* Fighter = Cast<AFighterBase>(OtherFighter);
	if (Fighter == nullptr) return;
	FVector facing = Fighter->GetActorForwardVector();
	if (facing.Y == WallFace.X)
	{
		FVector CurrVel = FVector(0.0f, 1500.f * WallFace.X, 500.f);
		Fighter->LaunchCharacter(CurrVel, true, true);
	}
}

// Called when the game starts or when spawned
void ABarrierBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrierBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABarrierBase::Wwise_OverlapBarrierTrigger(AActor* Fighter)
{
	AFighterBase* FB = Cast<AFighterBase>(Fighter);
	if (FB == nullptr) return;
	FB->Wwise_PlayBarrier();
}