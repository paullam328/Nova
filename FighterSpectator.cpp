// Copywrite Shapeshifter 2019

#include "FighterSpectator.h"
#include "CharacterCameraComp.h"
#include "FighterController.h"

// Sets default values
AFighterSpectator::AFighterSpectator()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CameraComp = CreateDefaultSubobject<UCharacterCameraComp>(TEXT("SpectatorCamera"));
	CameraComp->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AFighterSpectator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFighterSpectator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFighterSpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AFighterSpectator::InitializeSpectatorScreen_Validate()
{
	return true;
}

void AFighterSpectator::InitializeSpectatorScreen_Implementation()
{
	if (SpectatorScreen == nullptr && (IsLocallyControlled() || GetNetMode() == ENetMode::NM_ListenServer))
	{

		SpectatorScreen = CreateWidget<UUserWidget>(Cast<AFighterController>(GetController()), SpectatorScreenClass);

		if (SpectatorScreen != nullptr)
		{
			SpectatorScreen->AddToViewport();
		}
	}
}

