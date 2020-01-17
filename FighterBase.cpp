// Fill out your copyright notice in the Description page of Project Settings.
#include "FighterBase.h"
#include "UnrealNetwork.h"
#include "Engine.h"
#include "TimerManager.h"
#include "FighterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "gamePlayUserHUD.h"
#include "gamePlayUserWidget.h"
#include "PanelWidget.h"
#include "PlayerIconWidget.h"
#include "Engine/EngineTypes.h"
#include "StateMachineComponent.h"
#include "CharacterStateEnum.h"
#include "ProgressBar.h"
#include "HitboxComponent.h"
#include "AkGameplayStatics.h"
#include "AkAudioEvent.h"
#include "WinLoseScreen.h"
#include "NetworkingGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerScreen.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "FightingGameState.h"
#include "FighterPlayerState.h"
#include "ScriptDelegates.h"

#include "AkComponent.h"

#include "NetworkingGameInstance.h"
#include "FighterController.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"


// Sets default values
AFighterBase::AFighterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UFighterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	SM = CreateDefaultSubobject<UStateMachineComponent>(TEXT("SM"));

	DashRightParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DashRightParticleComponent"));

	DashLeftParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DashLeftParticleComponent"));

	SpecialAttack = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpecialAttack"));
	if (SpecialAttack->IsActive())
	{
		SpecialAttack->DeactivateSystem();
	}

	bAlwaysRelevant = true;
	NetPriority = 100.f;
	NetUpdateFrequency = 100.f;
	//JumpMaxCount = 2;
	PlayerArrow = CreateDefaultSubobject<UBillboardComponent>(TEXT("PlayerArrow"));
	PlayerArrow->AttachTo(RootComponent);

	myCap = GetCapsuleComponent();
	myCap->CanEditSimulatePhysics();
	myCap->SetIsReplicated(true);

}

void AFighterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//GetMesh()->bOnlyAllowAutonomousTickPose = 0;
}

// Called when the game starts or when spawned
void AFighterBase::BeginPlay()
{
	Super::BeginPlay();


	UWorld* World = GetWorld();

	if (World)
	{
		ENetMode Mode = World->GetNetMode();
		if (Mode == NM_DedicatedServer)
		{
			return;
		}
		else
		{

			if (UGameplayStatics::GetCurrentLevelName(World, true) != "Stage1")
			{

				bIsMovementEnabled = true;
			}
		}

	}

	SpawnGameplayWidgets();

	AdjustSFXSlider();
	//AdjustSFXSlider();
	//Multicast_SetSliderVals();

}

// Called every frame
void AFighterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%.2f"), Aggression));
	if (Health <= 0.0f)
	{
		bIsMovementEnabled = false;
		Server_StopAllAnims();
		bIsDead = true;
		if (DashLeftParticleComponent->IsActive())
		{
			DashLeftParticleComponent->Deactivate();
		}
		else if (DashRightParticleComponent->IsActive())
		{
			DashRightParticleComponent->Deactivate();
		}
		//play animation for being the big dead
	}
	//==========Paul's Wwise Stuff================

	//SFXSlider:
	/*if (!IsSFXSliderAdjusted)
	{
		AdjustSFXSlider();
	}*/

	//Ground check:
	UFighterMovementComponent* movement = Cast< UFighterMovementComponent>(GetMovementComponent());
	if (movement != nullptr)
	{
		if (movement->IsMovingOnGround())
		{
			if (isJumping)
			{
				isJumping = false;
				isLandingSoundTriggered = true;
				bInAir = false;
			}
		}
		else
		{
			if (bIsHitting == false)
			{
				bInAir = true;
				if (bIsMovementEnabled)
				{
					SM->SetCurrentState(ECharacterStates::AIR);
				}
			}
			if (DashLeftParticleComponent->IsActive())
			{
				DashLeftParticleComponent->Deactivate();
			}
			else if (DashRightParticleComponent->IsActive())
			{
				DashRightParticleComponent->Deactivate();
			}
		}
		if (isLandingSoundTriggered)
		{
			UAkGameplayStatics::PostEvent(landSound, this, false, "");
			isLandingSoundTriggered = false;
		}
	}

}

void AFighterBase::CalculateAggression()
{
	//A = (TD * (Hits/Miss))
	//float hitPercent = AttacksHit / AttacksThrown;
	//Aggression = DamageDone * hitPercent;
}


// Called to bind functionality to input
void AFighterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Movement Axis Bindings
	InputComponent->BindAxis("Right", this, &AFighterBase::Client_MoveCharacter);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFighterBase::StartJump);
	InputComponent->BindAction("Pause", IE_Pressed, this, &AFighterBase::Pause);
	InputComponent->BindAction("Dodge", IE_Pressed, this, &AFighterBase::Dodge);
}


/*
	This is for input action and Axis checks before sending the server notification of it happening.

*/
void AFighterBase::StartJump()
{
	if (bInAir == false && bIsMovementEnabled && bIsDodging == false)
	{
		bWantsToJump = true;
		SM->SetCurrentState(ECharacterStates::AIR);
		ServerWantsToJump();
	}
}


void AFighterBase::Client_DoJump_Implementation()
{
	//Instead of disabling input, make it disable move character instead

	if (bIsMovementEnabled)
	{
		SM->SetCurrentState(ECharacterStates::AIR);
		bInAir = true;
		bWantsToJump = false;
		this->Jump();
		DoJump();

		//==========Paul's Wwise Stuff================
		if (!isJumping)
		{
			UAkGameplayStatics::PostEvent(jumpSound, this, false, "");
			//====================

			isJumping = true;
		}
		//=============================================

	}
}
void AFighterBase::DoJumpAnalog(float val)
{
	if (val > 0.75)
	{
		StartJump();
	}
}
//Needs to be re written. causes errors with inputs i think.
void AFighterBase::Landed(const FHitResult& Hit)
{
	if (Hit.GetActor() == nullptr) return;

	if (Hit.GetActor()->IsA(AFighterBase::StaticClass())) return;
	bInAir = false;
	bHasDodged = false;
	if (SM->GetCurrentState() == ECharacterStates::HIT_AIR)
	{
		SM->SetCurrentState(ECharacterStates::HIT_GROUND);
	}
	else
	{
		if (SM->GetCurrentState() == ECharacterStates::ATK_AIR)
		{
			bLightAtkAir = false;
			bMedAtkAir = false;
			Server_LightToggle(ECharacterStates::ATK_AIR, false);
			if (bMedAtkAir)
			{
				Server_MedToggle(false, EMoveType::AIR_NEUTRAL);
			}
			else if (bMedAirUp)
			{
				Server_MedToggle(false, EMoveType::UP);
			}
			else if (bMedAirDown)
			{
				Server_MedToggle(false, EMoveType::DOWN);

			}
			
		}
		SM->SetCurrentState(ECharacterStates::GROUND);

	}

	FTransform SpawnLoc;
	FVector myLoc = this->GetActorLocation();
	myLoc.X -= 50.f;
	SpawnLoc.SetLocation(myLoc);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LandEffect, SpawnLoc, true, EPSCPoolMethod::None);
}
void AFighterBase::Pause()
{
	/*if (HUD != nullptr)
	{
		UGameplayStatics::GetPlayerController(this,0)->bShowMouseCursor = false;
		UGameplayStatics::GetPlayerController(this, 0)->bEnableClickEvents = false;
		UGameplayStatics::GetPlayerController(this, 0)->bEnableMouseOverEvents = false;
		HUD->Pause();
	}*/

}

void AFighterBase::Client_MoveCharacter_Implementation(float val)
{
	//====================

	//Instead of disabling input, make it disable move character instead
	if (bIsMovementEnabled)
	{
		DoMoveCharacter(val);
		if (val != 0.0f)
		{
			if (SM->GetCurrentState() == ECharacterStates::GROUND || SM->GetCurrentState() == ECharacterStates::AIR)
			{
				if (GetCharacterMovement()->IsMovingOnGround())
				{
					SM->SetCurrentState(ECharacterStates::GROUND);
				}

				if (val > 0.0f)
				{
					if (bIsFacingRight == false)
					{
						bIsFacingRight = true;
						//HitSplatComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitSplatSystem, SpawnLoc);

						if (DashLeftParticleComponent->bIsActive)
						{
							DashLeftParticleComponent->Deactivate();
						}
						UFighterMovementComponent* FighterCharacterMovement = Cast<UFighterMovementComponent>(GetCharacterMovement());

						if (FighterCharacterMovement != nullptr)
						{
							FighterCharacterMovement->bIsFacingRight = true;
						}
					}
				}
				//Move Left
				else if (val < 0.0f)
				{
					if (bIsFacingRight == true)
					{
						if (DashRightParticleComponent->bIsActive)
						{
							DashRightParticleComponent->Deactivate();
						}
						bIsFacingRight = false;
						UFighterMovementComponent* FighterCharacterMovement = Cast<UFighterMovementComponent>(GetCharacterMovement());

						if (FighterCharacterMovement != nullptr)
						{
							FighterCharacterMovement->bIsFacingRight = false;
						}
					}
				}
				if (bInAir == false)
				{
					if (val > 0.0f && DashRightParticleComponent->IsActive() == false)
					{
						DashRightParticleComponent->Activate();
						//Sound queue to start wwise and reinitialize
						SetIsDashStopPlayed(false);
						Wwise_PlayDashStart();
					}
					else if (val < 0.0f && DashLeftParticleComponent->IsActive() == false)
					{
						DashLeftParticleComponent->Activate();
						//sound queue to start wwise and reinitialize
						SetIsDashStopPlayed(false);
						Wwise_PlayDashStart();
					}
				}
				AddMovementInput(GetActorForwardVector(), FMath::Abs(val), false);
			}
		}
		else
		{
			if (DashRightParticleComponent->bIsActive)
			{
				DashRightParticleComponent->Deactivate();
				// play stop sound wwise
				Trigger_PlayDashStop();
			}
			if (DashLeftParticleComponent->bIsActive)
			{
				DashLeftParticleComponent->Deactivate();
				// play stop sound wwise
				Trigger_PlayDashStop();
			}
		}
	}

}

/* Add Checks to make sure its a valid move before processing it in server code.*/

/*Timers End*/

/*SERVER SIDE VALIDATION AND REPLICATION BELOW*/
void AFighterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFighterBase, bIsFacingRight);
	DOREPLIFETIME(AFighterBase, bLightAtk);
	DOREPLIFETIME(AFighterBase, bLightAtkAir);
	DOREPLIFETIME(AFighterBase, bMedAtk);
	DOREPLIFETIME(AFighterBase, bMedAtkAir);
	DOREPLIFETIME(AFighterBase, bSpecAtk);
	DOREPLIFETIME(AFighterBase, bIsDodging);
	DOREPLIFETIME(AFighterBase, bIsHitting);
	DOREPLIFETIME(AFighterBase, bHasHit);
	DOREPLIFETIME(AFighterBase, bWantsToJump);
	DOREPLIFETIME(AFighterBase, bInAir);
	DOREPLIFETIME(AFighterBase, bMedAirDown);
	DOREPLIFETIME(AFighterBase, bMedAirUp);
	DOREPLIFETIME(AFighterBase, bMedLauncher);
	DOREPLIFETIME(AFighterBase, BarsBuilt);

	DOREPLIFETIME(AFighterBase, Health);
	DOREPLIFETIME(AFighterBase, Aggression);
	DOREPLIFETIME(AFighterBase, DamageDone);
	DOREPLIFETIME(AFighterBase, AttacksThrown);
	DOREPLIFETIME(AFighterBase, AttacksHit);

	DOREPLIFETIME(AFighterBase, LeftThigh);
	DOREPLIFETIME(AFighterBase, LeftLeg);
	DOREPLIFETIME(AFighterBase, LeftFoot);
	DOREPLIFETIME(AFighterBase, LeftShoulder);
	DOREPLIFETIME(AFighterBase, LeftArm);
	DOREPLIFETIME(AFighterBase, LeftHand);

	DOREPLIFETIME(AFighterBase, RightThigh);
	DOREPLIFETIME(AFighterBase, RightLeg);
	DOREPLIFETIME(AFighterBase, RightFoot);
	DOREPLIFETIME(AFighterBase, RightShoulder);
	DOREPLIFETIME(AFighterBase, RightArm);
	DOREPLIFETIME(AFighterBase, RightHand);

	DOREPLIFETIME(AFighterBase, Head);
	DOREPLIFETIME(AFighterBase, UpperBody);
	DOREPLIFETIME(AFighterBase, LowerBody);

	DOREPLIFETIME(AFighterBase, SM);

	DOREPLIFETIME(AFighterBase, bIsDead);
	DOREPLIFETIME(AFighterBase, bIsInKnockback);
	DOREPLIFETIME(AFighterBase, Condition);
	DOREPLIFETIME(AFighterBase, IsConditionDecided);
	DOREPLIFETIME(AFighterBase, bIsMovementEnabled);
	DOREPLIFETIME(AFighterBase, bHitInAirFront);
	DOREPLIFETIME(AFighterBase, bHitInAirBack);
	DOREPLIFETIME(AFighterBase, bHitOnGround);

	DOREPLIFETIME(AFighterBase, bIsWinner);

	DOREPLIFETIME(AFighterBase, LightHitSplat);
	DOREPLIFETIME(AFighterBase, MedHitSplat);
	DOREPLIFETIME(AFighterBase, SpecHitSplat);
	DOREPLIFETIME(AFighterBase, SpecialAttack);
	DOREPLIFETIME(AFighterBase, LandEffect);
	DOREPLIFETIME(AFighterBase, IsDashStopPlayed);

	DOREPLIFETIME(AFighterBase, ArrowMeshComp);
	DOREPLIFETIME(AFighterBase, ArrowMat);
}

/*Movement*/
bool AFighterBase::DoJump_Validate()
{
	return true;
}
void AFighterBase::DoJump_Implementation()
{
	bWantsToJump = false;
	this->Jump();
	bInAir = true;
}
bool AFighterBase::ServerWantsToJump_Validate()
{
	return true;
}
void AFighterBase::ServerWantsToJump_Implementation()
{
	bWantsToJump = true;
}

bool AFighterBase::DoHitChar_Validate(UPrimitiveComponent* HittingComp, UPrimitiveComponent* HitComp, AActor* HitBy, EHitPower power, float DamageToDo, float KBTime, FVector_NetQuantize KBVec)
{
	return true;
}

void AFighterBase::DoHitChar_Implementation(UPrimitiveComponent* HittingComp, UPrimitiveComponent* HitComp, AActor* HitBy, EHitPower power, float DamageToDo, float KBTime, FVector_NetQuantize KBVec)
{
	FVector launchVec;
	if (this != nullptr)
	{
		if (bIsDodging) return;

		SM->SetCurrentState(ECharacterStates::HIT_AIR);

		/* OTHER HIT  */
		if (power != EHitPower::SPECIAL)
		{
			if (!IsValid(HitBy)) return;
			//Where does the special hit you D:
			if (HitBy->GetActorForwardVector() == this->GetActorForwardVector())
			{
				if (bHitInAirFront)
				{
					bHitInAirFront = true;
				}
				bHitInAirBack = true;
			}
			else
			{
				if (bHitInAirBack)
				{
					bHitInAirBack = false;
				}
				bHitInAirFront = true;
			}
			//Sets the vector for being launched based off the KB vec * the hitting actors forward vector
			launchVec = KBVec * HitBy->GetActorForwardVector();
			launchVec.Z = KBVec.Z;

		}
		else	
		{	/*SPECIAL ATTACK HIT*/
			if (HitBy == nullptr) return;

			if (HitBy->GetActorForwardVector() != this->GetActorForwardVector())
			{
				if (bHitInAirFront)
				{
					bHitInAirFront = false;
				}
				bHitInAirBack = true;
			}
			else
			{
				if (bHitInAirBack)
				{
					bHitInAirBack = false;
				}
				bHitInAirFront = true;
			}


			launchVec = KBVec * HitBy->GetActorForwardVector();
			launchVec.Z = KBVec.Z;
			this->LaunchCharacter(launchVec, true, true);
		}



		bIsMovementEnabled = false;
		Health -= DamageToDo;

		this->LaunchCharacter(launchVec, false, true);
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			World->GetTimerManager().SetTimer(TimerHandle_Knockback, this, &AFighterBase::Client_KBEnd, KBTime);
		}
		Trigger_HealthZero(this);

	}
}

void AFighterBase::Client_KBEnd_Implementation()
{
	bIsMovementEnabled = true;
	bHitInAirFront = false;
	bHitInAirBack = false;
	bHitOnGround = false;
	SM->KbEnded();
	this->Server_KBEnd();
}
bool AFighterBase::Server_KBEnd_Validate()
{
	return true;
}
void AFighterBase::Server_KBEnd_Implementation()
{
	bHitInAirFront = false;
	bHitInAirBack = false;
	bHitOnGround = false;
}

void AFighterBase::HitChar(UPrimitiveComponent* HittingComp, UPrimitiveComponent* HitComp, AActor* HitBy, EHitPower power, float DamageToDo, float KBTime, FVector_NetQuantize KBVec)
{
	if (this != nullptr)
	{
		DoHitChar(HitComp, HitComp, HitBy, power, DamageToDo, KBTime, KBVec);
	}
}
bool AFighterBase::SpawnHitSplat_Validate(UPrimitiveComponent* HitComp, EHitPower power)
{
	return true;
}
void AFighterBase::SpawnHitSplat_Implementation(UPrimitiveComponent* HitComp, EHitPower power)
{
	//Play sound here
	FTransform SpawnLoc;
	FVector loc = HitComp->GetSocketLocation(HitComp->GetAttachSocketName());
	loc.X -= 50.f;
	SpawnLoc.SetLocation(loc);
	FString name = HitComp->GetName();
	//Set a switch to pick what random hitsplat to play based off power.
	switch (power)
	{
	case EHitPower::LIGHT:
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LightHitSplat[FMath::Rand() % 2], SpawnLoc, true, EPSCPoolMethod::None);
		break;
	case EHitPower::MEDIUM:
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MedHitSplat[FMath::Rand() % 2], SpawnLoc, true, EPSCPoolMethod::None);
		break;
	case EHitPower::SPECIAL:
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpecHitSplat, SpawnLoc, true, EPSCPoolMethod::None);
		break;
	}
	
}
bool AFighterBase::DoMoveCharacter_Validate(float val)
{
	return true;
}
void AFighterBase::DoMoveCharacter_Implementation(float val)
{
	//Character Rotation Code
	//Move Right
	if (val > 0.0f)
	{
		if (bIsFacingRight == false)
		{
			bIsFacingRight = true;

			if (DashLeftParticleComponent->bIsActive)
			{
				DashLeftParticleComponent->Deactivate();
			}
			UFighterMovementComponent* FighterCharacterMovement = Cast<UFighterMovementComponent>(GetCharacterMovement());

			if (FighterCharacterMovement != nullptr)
			{
				FighterCharacterMovement->bIsFacingRight = true;
			}
		}
	}
	//Move Left
	else if (val < 0.0f)
	{
		if (bIsFacingRight == true)
		{
			if (DashRightParticleComponent->bIsActive)
			{
				DashRightParticleComponent->Deactivate();
			}
			bIsFacingRight = false;
			UFighterMovementComponent* FighterCharacterMovement = Cast<UFighterMovementComponent>(GetCharacterMovement());

			if (FighterCharacterMovement != nullptr)
			{
				FighterCharacterMovement->bIsFacingRight = false;
			}
		}
	}
	else
	{
		if (DashRightParticleComponent->bIsActive)
		{
			DashRightParticleComponent->Deactivate();
		}
		if (DashLeftParticleComponent->bIsActive)
		{
			DashLeftParticleComponent->Deactivate();
		}
	}
	if (bInAir == false)
	{
		if (val > 0.0f && DashRightParticleComponent->IsActive() == false)
		{
			if (GetVelocity().Size() != 0)
			{
				DashRightParticleComponent->Activate();
			}
		}
		else if (val < 0.0f && DashLeftParticleComponent->IsActive() == false)
		{
			if (GetVelocity().Size() != 0)
			{
				DashLeftParticleComponent->Activate();
			}
		}
	}

	AddMovementInput(GetActorForwardVector(), FMath::Abs(val), false);

}
bool AFighterBase::DisablePawnInput_Validate()
{
	return true;
}

void AFighterBase::DisablePawnInput_Implementation()
{
	DisableInput(Cast<APlayerController>(GetController()));
}


bool AFighterBase::Trigger_HealthZero_Validate(AFighterBase* fb)
{
	return true;
}

void AFighterBase::Trigger_HealthZero_Implementation(AFighterBase* fb)
{
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			if (Health <= 0)
			{
				Cast<AFightingGameState>(UGameplayStatics::GetGameState(World))->OnZeroHealthDecideWinLose(fb);
			}
		}
	}
}


void AFighterBase::Arms(bool isRightSide, bool isActive)
{
	if (isRightSide)
	{
		RightShoulder->SetCanDamage(isActive);
		RightArm->SetCanDamage(isActive);
		RightHand->SetCanDamage(isActive);
	}
	else
	{
		LeftShoulder->SetCanDamage(isActive);
		LeftArm->SetCanDamage(isActive);
		LeftHand->SetCanDamage(isActive);
	}
}

void AFighterBase::Legs(bool isRightSide, bool isActive)
{
	if (isRightSide)
	{
		RightThigh->SetCanDamage(isActive);
		RightLeg->SetCanDamage(isActive);
		RightFoot->SetCanDamage(isActive);
	}
	else
	{
		LeftThigh->SetCanDamage(isActive);
		LeftLeg->SetCanDamage(isActive);
		LeftFoot->SetCanDamage(isActive);
	}
}

void AFighterBase::Body(bool isActive)
{
	UpperBody->SetCanDamage(isActive);
	LowerBody->SetCanDamage(isActive);
}

void AFighterBase::HeadHitbox(bool isActive)
{
	Head->SetCanDamage(isActive);
}



#pragma region Light Attack
/*Call this before switching states on the end of the hit*/
bool AFighterBase::Server_LightToggle_Validate(ECharacterStates atkType, bool bAtkIsActive)
{
	return true;
}
void AFighterBase::Server_LightToggle_Implementation(ECharacterStates atkType, bool bAtkIsActive)
{

	switch (atkType)
	{
	case ECharacterStates::ATK_GROUND:
		bLightAtk = bAtkIsActive;
		if (bAtkIsActive == true)
		{
			SM->SetCurrentState(ECharacterStates::ATK_GROUND);
		}
		else
		{
			SM->SetCurrentState(ECharacterStates::GROUND);
		}
		break;
	case ECharacterStates::ATK_AIR:
		bLightAtkAir = bAtkIsActive;
		if (bAtkIsActive == true)
		{
			SM->SetCurrentState(ECharacterStates::ATK_AIR);
		}
		else
		{
			SM->SetCurrentState(ECharacterStates::AIR);
		}
		break;
	}


}
#pragma endregion

#pragma region Medium Attack
bool AFighterBase::Server_MedToggle_Validate(bool bAtkIsActive, EMoveType move)
{
	return true;
}
void AFighterBase::Server_MedToggle_Implementation(bool bAtkIsActive, EMoveType move)
{
	//Gets what moves being done based on an enum and sets to be true or false on the server.
	switch (move)
	{
	case EMoveType::NEUTRAL:
		bMedAtk = bAtkIsActive;
		if (bAtkIsActive == true)
		{
			SM->SetCurrentState(ECharacterStates::ATK_GROUND);
			bIsHitting = true;
		}
		else
		{
			SM->SetCurrentState(ECharacterStates::GROUND);
			bIsHitting = false;
		}
		break;
	case EMoveType::AIR_NEUTRAL:
		bMedAtkAir = bAtkIsActive;
		if (bAtkIsActive == true)
		{
			SM->SetCurrentState(ECharacterStates::ATK_AIR);
			bIsHitting = true;
		}
		else
		{
			SM->SetCurrentState(ECharacterStates::AIR);
			bIsHitting = false;
		}
		break;
	case EMoveType::ANTI_AIR:
		bMedLauncher = bAtkIsActive;
		if (bAtkIsActive == true)
		{
			SM->SetCurrentState(ECharacterStates::ATK_GROUND);
			bIsHitting = true;
		}
		else
		{
			SM->SetCurrentState(ECharacterStates::GROUND);
			bIsHitting = false;
		}
		break;
	case EMoveType::DOWN:
		bMedAirDown = bAtkIsActive;
		if (bAtkIsActive == true)
		{
			SM->SetCurrentState(ECharacterStates::ATK_AIR);
			bIsHitting = true;
		}
		else
		{
			SM->SetCurrentState(ECharacterStates::AIR);
			bIsHitting = false;
		}
		break;
	case EMoveType::UP:
		bMedAirUp = bAtkIsActive;
		if (bAtkIsActive == true)
		{
			SM->SetCurrentState(ECharacterStates::ATK_AIR);
			bIsHitting = true;
		}
		else
		{
			SM->SetCurrentState(ECharacterStates::AIR);
			bIsHitting = false;
		}
		break;
	}
}

#pragma endregion

bool AFighterBase::Server_DodgeToggle_Validate(bool bIsActive)
{
	return true;
}
void AFighterBase::Server_DodgeToggle_Implementation(bool bIsActive)
{
	bIsDodging = bIsActive;

}


bool AFighterBase::CreateWinLoseWidget_Validate(ECondition cond)
{
	return true;
}

void AFighterBase::CreateWinLoseWidget_Implementation(ECondition cond)
{
	if (WinLoseScreen == nullptr && IsLocallyControlled())
	{

		WinLoseScreen = CreateWidget<UWinLoseScreen>(GetWorld(), WinLoseScreenClass);

		WinLoseScreen->AddToViewport();

		//Note: Conditions need to be flipped to be correct, don't know why but whatever... 3 players are bugged, so don't test for 3 Players for now
		//Need a fix for this!!!

		WinLoseScreen->SetWinLoseText(cond);

	}
}

bool AFighterBase::Server_LaunchChar_Validate(FVector LaunchVelocity)
{
	return true;
}
void AFighterBase::Server_LaunchChar_Implementation(FVector LaunchVelocity)
{
	this->LaunchCharacter(LaunchVelocity, true, true);
}



bool AFighterBase::SpawnGameplayWidgets_Validate()
{
	return true;
}
void AFighterBase::SpawnGameplayWidgets_Implementation()
{
	//HUD = Cast<AgamePlayUserHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (ArrowTexture != nullptr && PlayerArrow != nullptr)
	{
		//PlayerArrow->SetSprite(ArrowTexture);
	}

}



bool AFighterBase::Multicast_SetSliderVals_Validate()
{
	return true;
}

void AFighterBase::Multicast_SetSliderVals_Implementation()
{
	//GetPlayerState<AFighterPlayerState>()->AdjustVolumeSlider();

	/*if (Role < ROLE_Authority)
	{
		UAkComponent * AkComp = UAkGameplayStatics::SpawnAkComponentAtLocation(GetWorld(), MainMenuEvent, nullptr, FVector(0, 0, 0), FRotator(0, 0, 0), true, "", "", false);
		FString str = "Mus_Slider";
		AkComp->SetRTPCValue(str, 0.5f, 0);
	}*/

}

void AFighterBase::AdjustSFXSlider()
{
	UNetworkingGameInstance* NGI = GetGameInstance<UNetworkingGameInstance>();
	if (NGI != nullptr)
	{
		UAkGameplayStatics::SetOutputBusVolume(NGI->SFXSliderVal, this);
		IsSFXSliderAdjusted = true;
	}
}

void AFighterBase::Dodge()
{
	if (bIsMovementEnabled == false) return;
	if (bHasDodged == true) return;

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%.2f"), InputComponent->GetAxisValue(FName("Jump"))));
		DodgeDistance = FVector(0.f, 0.0f, 0.0f);

		if (SM->GetCurrentState() == ECharacterStates::AIR)
		{
			//Right
			if (InputComponent->GetAxisValue(FName("Right")) >= .25f)
			{
				DodgeDistance.Y = 2000.f;

			}
			//Left 
			else if (InputComponent->GetAxisValue(FName("Right")) <= -.25f)
			{
				DodgeDistance.Y = -2000.0f;
			}
			else if (InputComponent->GetAxisValue(FName("Jump")) <= -.15f)
			{
				DodgeDistance = FVector(0.0f, 0.0f, -3000.0f);
			}

			LaunchCharacter(DodgeDistance, true, true);
			Server_LaunchChar(DodgeDistance);
			bIsDodging = true;
			Server_DodgeToggle(true);
			World->GetTimerManager().SetTimer(TimerHandle_Dodge, this, &AFighterBase::Client_OnDodgeEnd, DodgeTime);
			bHasDodged = true;
		}
		else
		{
			Client_StopMovement();
			bIsDodging = true;
			Server_DodgeToggle(true);
			World->GetTimerManager().SetTimer(TimerHandle_Dodge, this, &AFighterBase::Client_OnDodgeEnd, DodgeTime);
		}
	}
}

void AFighterBase::Client_OnDodgeEnd_Implementation()
{

	bIsDodging = false;
	Server_DodgeToggle(false);
}

void AFighterBase::Client_StopMovement_Implementation()
{
	GetCharacterMovement()->StopActiveMovement();
}


bool AFighterBase::Server_StopAllAnims_Validate()
{
	return true;
}

void AFighterBase::Server_StopAllAnims_Implementation()
{
	bLightAtk = false; //grounded light attack
	bMedAtk = false; //Grounded med attack
	bMedLauncher = false; // Launcher = Anti air for med attacks
	bLightAtkAir = false; //Standard air attack
	bMedAtkAir = false; //standard air launcher
	bMedAirDown = false; //Downward air launcher
	bMedAirUp = false; //Upwards med air launcher
	bSpecAtk = false;
}

bool AFighterBase::Wwise_PlayDashStart_Validate()
{
	return true;
}
void AFighterBase::Wwise_PlayDashStart_Implementation()
{
	UAkGameplayStatics::PostEvent(DashStart, this, false, "");
}

bool AFighterBase::Wwise_PlayDashStop_Validate()
{
	return true;
}
void AFighterBase::Wwise_PlayDashStop_Implementation()
{
	UAkGameplayStatics::PostEvent(DashStop, this, false, "");
}

bool AFighterBase::SetIsDashStopPlayed_Validate(bool isPlayed)
{
	return true;
}

void AFighterBase::SetIsDashStopPlayed_Implementation(bool isPlayed)
{
	if (Role == ROLE_Authority)
	{
		IsDashStopPlayed = isPlayed;
	}
}

bool AFighterBase::Trigger_PlayDashStop_Validate()
{
	return true;
}

void AFighterBase::Trigger_PlayDashStop_Implementation()
{
	if (Role == ROLE_Authority)
	{
		if (!IsDashStopPlayed)
		{
			Wwise_PlayDashStop();
			SetIsDashStopPlayed(true);
		}
	}
}

//bool AFighterBase::Wwise_PlayBarrier_Validate()
//{
//	return true;
//}

void AFighterBase::Wwise_PlayBarrier()/*_Implementation()*/
{
	UAkGameplayStatics::PostEvent(BarrierHit, this, false, "");
}

void AFighterBase::OnRep_SetArrowMaterial()
{
	ArrowMeshComp->SetMaterial(0, ArrowMat);
}

bool AFighterBase::SetArrowMaterial_Validate(UStaticMeshComponent* SMC)
{
	return true;
}

void AFighterBase::SetArrowMaterial_Implementation(UStaticMeshComponent* SMC)
{
	AFighterPlayerState* FPS = GetPlayerState<AFighterPlayerState>();
	ArrowMeshComp = SMC;
	if (FPS != nullptr)
	{
		if (ArrowMats.IsValidIndex(FPS->PlayerIndex))
		{
			switch (FPS->PlayerIndex)
			{
				case 0:
					ArrowMat = ArrowMats[0];
					OnRep_SetArrowMaterial();
					break;
				case 1:
					ArrowMat = ArrowMats[1];
					OnRep_SetArrowMaterial();
					break;
				case 2:
					ArrowMat = ArrowMats[2];
					OnRep_SetArrowMaterial();
					break;
				case 3:
					ArrowMat = ArrowMats[3];
					OnRep_SetArrowMaterial();
					break;
				case 4:
					ArrowMat = ArrowMats[4];
					OnRep_SetArrowMaterial();
					break;
				case 5:
					ArrowMat = ArrowMats[5];
					OnRep_SetArrowMaterial();
					break;
			}
		}
	}
}