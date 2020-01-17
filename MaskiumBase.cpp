// Copywrite Shapeshifter 2019

#include "MaskiumBase.h"
#include "FighterBase.h"
#include "UnrealNetwork.h"
#include "Engine.h"
#include "TimerManager.h"
#include "FighterMovementComponent.h"
#include "HitboxComponent.h"
#include "Engine/EngineTypes.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"
#include "StateMachineComponent.h"
#include "Materials/MaterialInterface.h"
#include "Demark_Special.h"
#include "FighterPlayerState.h"
#include "CharacterCameraComp.h"


#pragma region UnrealClassStuff
AMaskiumBase::AMaskiumBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UFighterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	myCamera = CreateDefaultSubobject<UCharacterCameraComp>(TEXT("myCamera"));
	myCamera->AttachTo(RootComponent);
	SetupHitboxComponents();
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
}

void AMaskiumBase::SetupHitboxComponents()
{
	/* B O D Y */
	Head = CreateDefaultSubobject<UHitboxComponent>(TEXT("Head"));
	UpperBody = CreateDefaultSubobject<UHitboxComponent>(TEXT("UpperBody"));
	LowerBody = CreateDefaultSubobject<UHitboxComponent>(TEXT("LowerBody"));
	/* A R M S */

	//Left Side
	LeftHand = CreateDefaultSubobject<UHitboxComponent>(TEXT("LeftHand"));
	LeftHand->OnComponentBeginOverlap.AddDynamic(this, &AMaskiumBase::OnOverlap);
	LeftHand->OnComponentEndOverlap.AddDynamic(this, &AMaskiumBase::OnOverlapEnd);

	LeftArm = CreateDefaultSubobject<UHitboxComponent>(TEXT("LeftArm"));
	LeftArm->OnComponentBeginOverlap.AddDynamic(this, &AMaskiumBase::OnOverlap);
	LeftArm->OnComponentEndOverlap.AddDynamic(this, &AMaskiumBase::OnOverlapEnd);

	LeftShoulder = CreateDefaultSubobject<UHitboxComponent>(TEXT("LeftShoulder"));


	//Right side
	RightHand = CreateDefaultSubobject<UHitboxComponent>(TEXT("RightHand"));
	RightHand->OnComponentBeginOverlap.AddDynamic(this, &AMaskiumBase::OnOverlap);
	RightHand->OnComponentEndOverlap.AddDynamic(this, &AMaskiumBase::OnOverlapEnd);

	RightArm = CreateDefaultSubobject<UHitboxComponent>(TEXT("RightArm"));
	RightArm->OnComponentBeginOverlap.AddDynamic(this, &AMaskiumBase::OnOverlap);
	RightArm->OnComponentEndOverlap.AddDynamic(this, &AMaskiumBase::OnOverlapEnd);

	RightShoulder = CreateDefaultSubobject<UHitboxComponent>(TEXT("RightShoulder"));

	/* L E G S */

	//Left Side
	LeftThigh = CreateDefaultSubobject<UHitboxComponent>(TEXT("LeftThigh"));

	LeftLeg = CreateDefaultSubobject<UHitboxComponent>(TEXT("LeftLeg"));
	LeftLeg->OnComponentBeginOverlap.AddDynamic(this, &AMaskiumBase::OnOverlap);
	LeftLeg->OnComponentEndOverlap.AddDynamic(this, &AMaskiumBase::OnOverlapEnd);

	LeftFoot = CreateDefaultSubobject<UHitboxComponent>(TEXT("LeftFoot"));
	LeftFoot->OnComponentBeginOverlap.AddDynamic(this, &AMaskiumBase::OnOverlap);
	LeftFoot->OnComponentEndOverlap.AddDynamic(this, &AMaskiumBase::OnOverlapEnd);

	//Right side
	RightThigh = CreateDefaultSubobject<UHitboxComponent>(TEXT("RightThigh"));

	RightLeg = CreateDefaultSubobject<UHitboxComponent>(TEXT("RightLeg"));
	RightLeg->OnComponentBeginOverlap.AddDynamic(this, &AMaskiumBase::OnOverlap);
	RightLeg->OnComponentEndOverlap.AddDynamic(this, &AMaskiumBase::OnOverlapEnd);

	RightFoot = CreateDefaultSubobject<UHitboxComponent>(TEXT("RightFoot"));
	RightFoot->OnComponentBeginOverlap.AddDynamic(this, &AMaskiumBase::OnOverlap);
	RightFoot->OnComponentEndOverlap.AddDynamic(this, &AMaskiumBase::OnOverlapEnd);
}

void AMaskiumBase::MakeHitboxesVisible(bool setting)
{
	/* B O D Y */
	Head->SetHiddenInGame(setting);
	UpperBody->SetHiddenInGame(setting);
	LowerBody->SetHiddenInGame(setting);

	/* A R M S */
	LeftHand->SetHiddenInGame(setting);
	LeftArm->SetHiddenInGame(setting);
	LeftShoulder->SetHiddenInGame(setting);

	RightHand->SetHiddenInGame(setting);
	RightArm->SetHiddenInGame(setting);
	RightShoulder->SetHiddenInGame(setting);

	/* L E G S */
	LeftThigh->SetHiddenInGame(setting);
	LeftLeg->SetHiddenInGame(setting);
	LeftFoot->SetHiddenInGame(setting);

	RightThigh->SetHiddenInGame(setting);
	RightLeg->SetHiddenInGame(setting);
	RightFoot->SetHiddenInGame(setting);
}



void AMaskiumBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	const FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	USkeletalMeshComponent* MyMesh = this->GetMesh();
	if (MyMesh != nullptr && MyMesh->IsValidLowLevelFast())
	{
		SpecialAttack->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("RHand")));
		DashRightParticleComponent->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("RFoot")));
		DashLeftParticleComponent->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("LFoot")));

		if (!IsValid(Head))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		Head->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("Head")));


		if (!IsValid(UpperBody))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		UpperBody->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("UpperBody")));


		if (!IsValid(LowerBody))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		LowerBody->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("LowerBody")));


		if (!IsValid(LeftHand))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		LeftHand->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("LHand")));


		if (!IsValid(LeftArm))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		LeftArm->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("LArm")));

		if (!IsValid(LeftShoulder))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		LeftShoulder->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("LShoulder")));

		if (!IsValid(LeftThigh))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		LeftThigh->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("LThigh")));

		if (!IsValid(LeftLeg))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}

		LeftLeg->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("LLeg")));
		if (!IsValid(LeftFoot))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		LeftFoot->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("LFoot")));

		if (!IsValid(RightHand))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		RightHand->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("RHand")));
		if (!IsValid(RightArm))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		RightArm->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("RArm")));
		if (!IsValid(RightShoulder))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		RightShoulder->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("RShoulder")));
		if (!IsValid(RightThigh))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		RightThigh->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("RThigh")));
		if (!IsValid(RightLeg))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		RightLeg->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("RLeg")));

		if (!IsValid(RightFoot))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")));
			return;
		}
		RightFoot->AttachToComponent(MyMesh, attachmentRules, FName(TEXT("RFoot")));

	}

	
}



void AMaskiumBase::BeginPlay()
{
	Super::BeginPlay();
	MakeHitboxesVisible(!ShowHitboxes);
	
}


void AMaskiumBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMaskiumBase::Landed(const FHitResult &Hit)
{
	Super::Landed(Hit);

	CanUpAttack = true;
}

void AMaskiumBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Action binds
	InputComponent->BindAction("Light", IE_Pressed, this, &AMaskiumBase::LightAtk);
	InputComponent->BindAction("Medium", IE_Pressed, this, &AMaskiumBase::MedAtk);
	InputComponent->BindAction("Special", IE_Pressed, this, &AMaskiumBase::SpecAtk);
	InputComponent->BindAxis("DirectionInput", this, &AMaskiumBase::TestFunc);
}
#pragma endregion
void AMaskiumBase::TestFunc(float val)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%.2f"), val));
}
#pragma region Setups Before Network action

void AMaskiumBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMaskiumBase, PlayerNum);
	DOREPLIFETIME(AMaskiumBase, SelectedHelmColour);
	DOREPLIFETIME(AMaskiumBase, SelectedCapeColour);
	DOREPLIFETIME(AMaskiumBase, SelectedBodyColour);
	DOREPLIFETIME(AMaskiumBase, ProjectileRight);
	DOREPLIFETIME(AMaskiumBase, ProjectileLeft);
}

void AMaskiumBase::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	UHitboxComponent* MyHittingComp = Cast<UHitboxComponent>(OverlappedComp);
	if (MyHittingComp != nullptr)
	{
		if (MyHittingComp->GetCanDamage())
		{
			if (bIsHitting)
			{
				if (OtherActor != this && OtherActor != nullptr)
				{
					DoOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, sweepResult);
					UWorld* World = GetWorld();
					if (World != nullptr)
					{
						World->GetTimerManager().SetTimer(PostHitTimer, this, &AMaskiumBase::OnEndHit, .1f);
					}
				}
			}
		}
	}
}

void AMaskiumBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}



void AMaskiumBase::LightAtk()
{
	if (bIsMovementEnabled == false) return;
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (SM->GetCurrentState() == ECharacterStates::GROUND)
		{
			PlayLightSound();
			SM->SetCurrentState(ECharacterStates::ATK_GROUND);
			Client_StopMovement();
			World->GetTimerManager().SetTimer(TimerHandle_LightPunch, this, &AMaskiumBase::Client_OnLightAttackEnd, LightAtkTime);
			bLightAtk = true;
			Server_LightToggle(SM->GetCurrentState(), true);
		}
		else if (SM->GetCurrentState() == ECharacterStates::AIR)
		{
			SM->SetCurrentState(ECharacterStates::ATK_AIR);
			World->GetTimerManager().SetTimer(TimerHandle_LightAir, this, &AMaskiumBase::Client_OnLightAttackEnd, LightAirTime);
			bLightAtkAir = true;
			Server_LightToggle(SM->GetCurrentState(), true);
		}


		AttacksThrown++;
		bIsHitting = true;

	}
}

void AMaskiumBase::MedAtk()
{
	if (bIsMovementEnabled == false) return;
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		PlayMedSound();
		float axisVal = InputComponent->GetAxisValue("DirectionInput");
		if (SM->GetCurrentState() == ECharacterStates::GROUND || SM->GetCurrentState() == ECharacterStates::ATK_GROUND)
		{
			if (axisVal >= -.30f)
			{
				Client_StopMovement();
				//Do netural medium attack
				bMedAtk = true;
				bIsMovementEnabled = false;
				Server_MedToggle(true, EMoveType::NEUTRAL);
				World->GetTimerManager().SetTimer(TimerHandle_MedGround, this, &AMaskiumBase::Client_OnMediumAttackEnd, MedAtkTime);
				FVector launchVec = FVector(0.0f, 1500.f, 0.0f) * this->GetActorForwardVector();
				this->LaunchCharacter(launchVec, false, false);
				Server_LaunchChar(launchVec);
			}
			else if (axisVal < -.30f)
			{
				//Do up medium anti air
				bMedLauncher = true;
				Server_MedToggle(true, EMoveType::ANTI_AIR);
				World->GetTimerManager().SetTimer(TimerHandle_MedLauncher, this, &AMaskiumBase::Client_OnMediumAttackEnd, MedLauncherTime);
				FVector launchVec = FVector(0.0f, 500.f, 0.0f) * this->GetActorForwardVector();
				this->LaunchCharacter(launchVec, false, false);
				Server_LaunchChar(launchVec);
			}

		}
		else if (SM->GetCurrentState() == ECharacterStates::AIR || SM->GetCurrentState() == ECharacterStates::ATK_AIR)
		{
			if (axisVal <= .30f && axisVal >= -.30f)
			{
				//DO Neutral med air
				bMedAtkAir = true;
				Server_MedToggle(true, EMoveType::AIR_NEUTRAL);
				World->GetTimerManager().SetTimer(TimerHandle_MedAirNeutral, this, &AMaskiumBase::Client_OnMediumAttackEnd, MedAirNeutralTime);
				FVector launchVec = FVector(0.0f, 200.f, 0.0f) * this->GetActorForwardVector();
				this->LaunchCharacter(launchVec, false, false);
				Server_LaunchChar(launchVec);
			}
			else if (axisVal <= -.30f)
			{
				if (CanUpAttack == false) return;
				//Do med up atk
				bMedAirUp = true;
				CanUpAttack = false;
				Server_MedToggle(true, EMoveType::UP);
				World->GetTimerManager().SetTimer(TimerHandle_MedAirUp, this, &AMaskiumBase::Client_OnMediumAttackEnd, MedAirUpTime);
				FVector launchVec = FVector(0.0f, 350.f, 0.0f) * this->GetActorForwardVector();
				launchVec.Z = 500.f;
				this->LaunchCharacter(launchVec, false, false);
				Server_LaunchChar(launchVec);
			}
			else if (axisVal >= .30f)
			{
				//do med down atk
				bMedAirDown = true;
				Server_MedToggle(true, EMoveType::DOWN);
				World->GetTimerManager().SetTimer(TimerHandle_MedAirDown, this, &AMaskiumBase::Client_OnMediumAttackEnd, MedAirDownTime);
			}
		}
		bIsHitting = true;

	}
}
void AMaskiumBase::SpecAtk()
{
	if (bIsMovementEnabled == false) return;
	if (SM->GetCurrentState() == ECharacterStates::AIR || SM->GetCurrentState() == ECharacterStates::ATK_AIR || SM->GetCurrentState() == ECharacterStates::HIT_AIR) return;
	if (Aggression < 100.f && BarsBuilt < 1) return;

	if (Aggression != 100)
	{
		Aggression += 100;
		BarsBuilt -= 1;
		Aggression -= 100;
	}
	//UGameplayStatics::SpawnEmitterAttached(BeamSystem,nullptr, FName("RHand"), SpawnLoc.GetLocation(), SpawnLoc.GetRotation(), EAttachLocation::SnapToTarget, true, EPSCPoolMethod::None);
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		Client_StopMovement();
		bIsMovementEnabled = false;
		World->GetTimerManager().SetTimer(TimerHandle_SpecialAttack, this, &AMaskiumBase::Client_OnSpecialAttackEnd, SpecialTime);
		World->GetTimerManager().SetTimer(TimerHandle_ShootProj, this, &AMaskiumBase::SpawnSpecActor, SpecialTime - .65f);
	}
	Server_SpecToggle(true);
}



#pragma endregion


#pragma region Timerz


void AMaskiumBase::Client_OnLightAttackEnd_Implementation()
{

	if (bLightAtk)
	{
		SM->SetCurrentState(ECharacterStates::GROUND);
		Server_LightToggle(ECharacterStates::ATK_GROUND, false);
	}
	else if (bLightAtkAir)
	{
		SM->SetCurrentState(ECharacterStates::AIR);
		Server_LightToggle(ECharacterStates::ATK_AIR, false);
	}
	ENetRole myRole = Role;
	bLightAtk = false;
	bLightAtkAir = false;
	bIsHitting = false;
	bHasHit = false;
}

void AMaskiumBase::Client_OnMediumAttackEnd_Implementation()
{

	if (bMedAtk)
	{
		SM->SetCurrentState(ECharacterStates::GROUND);
		Server_MedToggle(false, EMoveType::NEUTRAL);
		bMedAtk = false;
		bIsMovementEnabled = true;
	}
	else if (bMedAtkAir)
	{
		SM->SetCurrentState(ECharacterStates::AIR);
		Server_MedToggle( false, EMoveType::AIR_NEUTRAL);
		bMedAtkAir = false;
	}
	else if (bMedLauncher)
	{
		SM->SetCurrentState(ECharacterStates::GROUND);
		Server_MedToggle(false, EMoveType::ANTI_AIR);
		bMedLauncher = false;
	}
	else if (bMedAirDown)
	{
		SM->SetCurrentState(ECharacterStates::AIR);
		Server_MedToggle(false, EMoveType::DOWN);
		bMedAirDown = false;
	}
	else if (bMedAirUp)
	{
		SM->SetCurrentState(ECharacterStates::AIR);
		Server_MedToggle(false, EMoveType::UP);
		bMedAirUp = false;
	}

	ENetRole myRole = Role;
	bIsHitting = false;
	bHasHit = false;
}

void AMaskiumBase::Client_OnSpecialAttackEnd_Implementation()
{
	bIsHitting = false;
	bSpecAtk = false;
	bHasHit = false;
	bIsMovementEnabled = true;
	Server_SpecToggle(false);
}
#pragma endregion

#pragma region On End Hit
bool AMaskiumBase::OnEndHit_Validate()
{
	return true;
}
void AMaskiumBase::OnEndHit_Implementation()
{
	bHasHit = false;
	bIsHitting = false;
}
#pragma endregion

#pragma region Overlap

bool AMaskiumBase::DoOverlap_Validate(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	return true;
}
void AMaskiumBase::DoOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (bHasHit == false)
	{
		UHitboxComponent* MyHittingComp = Cast<UHitboxComponent>(OverlappedComp);

		if (IsValid(MyHittingComp) && IsValid(OtherComp))
		{
			if (MyHittingComp->GetCanDamage())
			{

				AFighterBase* OtherFighter = Cast<AFighterBase>(OtherActor);
				if (!IsValid(OtherFighter)) return;

				if (bLightAtk) /*Light Ground Attack*/
				{
					//Play the hitting sound
					PlayLightHitSound();
					//Spawn hit splat
					SpawnHitSplat(OtherComp, EHitPower::LIGHT);
					//If you have over 500 aggression do more damage
					if (BarsBuilt >= 5)
					{
						//Hit the character
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::LIGHT, Damage+ .25, LightGroundKBTime, LightGroundKB);
						if (Aggression >= 7)
						{
							Aggression -= 7;
						}
						else
						{
							BarsBuilt -= 1;
							Aggression += 100;
							Aggression -= 7;
						}
					}
					else
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::LIGHT, Damage, LightGroundKBTime, LightGroundKB);
					}
					//add aggression
					Aggression += 10;

				}
				else if (bMedAtk)
				{
					//Play the hitting sound
					PlayMedHitSound();
					//Spawn hit splat
					SpawnHitSplat(OtherComp, EHitPower::MEDIUM);
					//Hit the character
					if (BarsBuilt >= 5)
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + 1, MedGroundKBTime, MedGroundKB);
						if (Aggression >= 15)
						{
							Aggression -= 15;
						}
						else
						{
							BarsBuilt -= 1;
							Aggression += 100;
							Aggression -= 15;
						}
					}
					else
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + .5, MedGroundKBTime, MedGroundKB);
					}
					//add aggression
					Aggression += 18;
				}
				else if (bLightAtkAir)
				{
					if (OtherFighter->MedLauncher() || OtherFighter->MedAirUpAtk()) return;
					//Play the hitting sound
					PlayLightHitSound();
					//Spawn hit splat
					SpawnHitSplat(OtherComp, EHitPower::LIGHT);
					//Hit the character
					if (BarsBuilt >= 5)
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::LIGHT, Damage + 2, LightAirKBTime, LightAirKB);
						if (Aggression >= 7)
						{
							Aggression -= 7;
						}
						else
						{
							BarsBuilt -= 1;
							Aggression += 100;
							Aggression -= 7;
						}
					}
					else
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::LIGHT, Damage + .75, LightAirKBTime, LightAirKB);
					}
					//add aggression
					Aggression += 15;
					
				}
				else if (bMedAtkAir)
				{
					if (OtherFighter->MedLauncher() || OtherFighter->MedAirUpAtk()) return;
					//Play the hitting sound
					PlayMedHitSound();
					//Spawn hit splat
					SpawnHitSplat(OtherComp, EHitPower::MEDIUM);
					//Hit the character
					if (BarsBuilt >= 5)
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + 3, MedAirNeutralKBTime, MedAirKB);
						if (Aggression >= 9)
						{
							Aggression -= 9;
						}
						else
						{
							BarsBuilt -= 1;
							Aggression += 100;
							Aggression -= 9;
						}
					}
					else
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + .88, MedAirNeutralKBTime, MedAirKB);
					}
					//add aggression
					Aggression += 20;
					
				}
				else if (bMedLauncher)
				{
					/*ANTI AIR*/
					if (OtherFighter->SM->GetCurrentState() == ECharacterStates::ATK_AIR || OtherFighter->SM->GetCurrentState() == ECharacterStates::AIR)
					{
						//Play the hitting sound
						PlayMedHitSound();
						//Spawn hit splat
						SpawnHitSplat(OtherComp, EHitPower::MEDIUM);
						//Hit the character
						if (BarsBuilt >= 5)
						{
							OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + 1, MedAirNeutralKBTime, MedAirKB);
							if (Aggression >= 10)
							{
								Aggression -= 10;
							}
							else
							{
								BarsBuilt -= 1;
								Aggression += 100;
								Aggression -= 10;
							}
						}
						else
						{
							OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + .3, MedAirNeutralKBTime, MedAirKB);
						}
						//add aggression
						Aggression += 25;
						
					}
				}
				else if (bMedAirDown)
				{
					if (OtherFighter->MedLauncher() || OtherFighter->MedAirUpAtk()) return;
					//Play the hitting sound
					PlayMedHitSound();
					//Spawn hit splat
					SpawnHitSplat(OtherComp, EHitPower::MEDIUM);
					//Hit the character
					if (BarsBuilt >= 5)
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + 1.25, MedAirDownKBTime, MedAirDownKB);
						if (Aggression >= 10)
						{
							Aggression -= 10;
						}
						else
						{
							BarsBuilt -= 1;
							Aggression += 100;
							Aggression -= 10;
						}
					}
					else
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + 2, MedAirDownKBTime, MedAirDownKB);
					}
					//add aggression
					Aggression += 18;
					
				}
				else if (bMedAirUp)
				{
					if (OtherFighter->MedLauncher()) return;
					//Play the hitting sound
					PlayMedHitSound();
					//Spawn hit splat
					SpawnHitSplat(OtherComp, EHitPower::MEDIUM);
					//Hit the character
					if (BarsBuilt >= 5)
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + 1.4, MedAirUpKBTime, MedAirUpKB);
						if (Aggression >= 10)
						{
							Aggression -= 10;
						}
						else
						{
							BarsBuilt -= 1;
							Aggression += 100;
							Aggression -= 10;
						}
					}
					else
					{
						OtherFighter->HitChar(MyHittingComp, OtherComp, this, EHitPower::MEDIUM, Damage + 1.1, MedAirUpKBTime, MedAirUpKB);
					}
					//add aggression
					Aggression += 18;
				}
				bHasHit = true; //Need to find a way to change this after the hit finishes?
				if (Aggression >= 100)
				{
					if (BarsBuilt < 9)
					{
						Aggression = Aggression - 100;
						BarsBuilt++;
					}
					else
					{
						Aggression = 100;
					}
				}
			}

			else
			{
				FString firstPrint = MyHittingComp->GetName();
			}
		}
	}

}

#pragma endregion



#pragma region Wwise

//==========Paul's Wwise Stuff================
bool AMaskiumBase::PlayLightSound_Validate()
{
	return true;
}


void AMaskiumBase::PlayLightSound_Implementation()
{
	UAkGameplayStatics::PostEvent(lightAtkSound, this, false, "");
}

bool AMaskiumBase::PlayMedSound_Validate()
{
	return true;
}

void AMaskiumBase::PlayMedSound_Implementation()
{
	UAkGameplayStatics::PostEvent(medAtkSound, this, false, "");
}

bool AMaskiumBase::PlayLightAirSound_Validate()
{
	return true;
}

void AMaskiumBase::PlayLightAirSound_Implementation()
{
	UAkGameplayStatics::PostEvent(lightAirAtkSound, this, false, "");
}

bool AMaskiumBase::PlayMedAirSound_Validate()
{
	return true;
}

void AMaskiumBase::PlayMedAirSound_Implementation()
{
	UAkGameplayStatics::PostEvent(medAirAtkSound, this, false, "");
}

bool AMaskiumBase::PlaySpSound_Validate()
{
	return true;
}

void AMaskiumBase::PlaySpSound_Implementation()
{
	UAkGameplayStatics::PostEvent(spAtkSound, this, false, "");
}

bool AMaskiumBase::PlaySpAtkAnimSound_Validate()
{
	return true;
}

void AMaskiumBase::PlaySpAtkAnimSound_Implementation()
{
	UAkGameplayStatics::PostEvent(spAtkAnimSound, this, false, "");
}

bool AMaskiumBase::PlayLightHitSound_Validate()
{
	return true;
}
void AMaskiumBase::PlayLightHitSound_Implementation()
{
	UAkGameplayStatics::PostEvent(lightHitSound, this, false, "");
}

bool AMaskiumBase::PlayMedHitSound_Validate()
{
	return true;
}
void AMaskiumBase::PlayMedHitSound_Implementation()
{
	UAkGameplayStatics::PostEvent(medHitSound, this, false, "");
}

bool AMaskiumBase::PlaySpecHitSound_Validate()
{
	return true;
}
void AMaskiumBase::PlaySpecHitSound_Implementation()
{
	UAkGameplayStatics::PostEvent(spAtHitSound, this, false, "");
}

bool AMaskiumBase::PlayDeadSound_Validate()
{
	return true;
}
void AMaskiumBase::PlayDeadSound_Implementation()
{
	UAkGameplayStatics::PostEvent(deadSound, this, false, "");
}
//=============================================

#pragma endregion


#pragma region CharacterColours
//Here you set the color triggered by the PLAYERSTATE!  As the player is spawned BEFORE this was called in beginplay:

void AMaskiumBase::OnRep_SetCharColour()
{
	this->GetMesh()->SetMaterial(0, SelectedBodyColour);
	this->GetMesh()->SetMaterial(1, SelectedCapeColour);
	this->GetMesh()->SetMaterial(2, SelectedHelmColour);
}

bool AMaskiumBase::SERVER_SetCharColor_Validate(int index)
{
	return true;
}

void AMaskiumBase::SERVER_SetCharColor_Implementation(int index)
{

	switch (index)
	{
	case 0:
		SelectedBodyColour = BodyColour[0];
		SelectedCapeColour = CapeColour[0];
		SelectedHelmColour = HelmColour[0];
		OnRep_SetCharColour();
		break;
	case 1:
	
		SelectedBodyColour = BodyColour[1];
		SelectedCapeColour = CapeColour[1];
		SelectedHelmColour = HelmColour[1];
		OnRep_SetCharColour();
		break;
	
	case 2:
	
		SelectedBodyColour = BodyColour[2];
		SelectedCapeColour = CapeColour[2];
		SelectedHelmColour = HelmColour[2];
		OnRep_SetCharColour();
		break;
	
	case 3:
	
		SelectedBodyColour = BodyColour[3];
		SelectedCapeColour = CapeColour[3];
		SelectedHelmColour = HelmColour[3];
		OnRep_SetCharColour();
		break;
	case 4:
		SelectedBodyColour = BodyColour[4];
		SelectedCapeColour = CapeColour[4];
		SelectedHelmColour = HelmColour[4];
		OnRep_SetCharColour();
		break;
	case 5:
		SelectedBodyColour = BodyColour[5];
		SelectedCapeColour = CapeColour[5];
		SelectedHelmColour = HelmColour[5];
		OnRep_SetCharColour();
		break;
	}
	
}
#pragma endregion


#pragma region Spec Attack
bool AMaskiumBase::Server_SpecToggle_Validate(bool bAtkISActive)
{
	return true;
}

void AMaskiumBase::Server_SpecToggle_Implementation(bool bAtkIsActive)
{

	bSpecAtk = bAtkIsActive;

	if (bAtkIsActive)
	{
		SM->SetCurrentState(ECharacterStates::ATK_GROUND);
	}
	else
	{
		SM->SetCurrentState(ECharacterStates::GROUND);
	}
}

bool AMaskiumBase::SpawnSpecActor_Validate()
{
	return true;
}
void AMaskiumBase::SpawnSpecActor_Implementation()
{
	ENetRole myRole = Role;
	//Spawning Projectile
	FVector loc = GetMesh()->GetSocketLocation(FName("RHand"));
	loc.Y += (75 * this->GetActorForwardVector().Y);
	UFighterMovementComponent* movement = Cast<UFighterMovementComponent>(this->GetMovementComponent());
	//Rotation of the projectile
	if (movement != nullptr)
	{
		if (movement->bIsFacingRight)
		{
			ADemark_Special* MySpec = this->GetWorld()->SpawnActor <ADemark_Special>(ProjectileRight, loc, this->GetActorRotation());
		}
		else
		{
			ADemark_Special* MySpec = this->GetWorld()->SpawnActor <ADemark_Special>(ProjectileLeft, loc, this->GetActorRotation());
		}
	}
	PlaySpSound();
}
#pragma endregion