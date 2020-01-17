// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "Condition.h"
#include "CharacterStateEnum.h"
#include "Materials/MaterialInterface.h"
#include "FighterBase.generated.h"

class USphereComponent;
class UCapsuleComponent;
class AgamePlayUserHUD;
class UgamePlayUserWidget;
class UHitboxComponent;
class UAkComponent;
class UAkAudioEvent;
class UWinLoseScreen;
class UStateMachineComponent;
class UPlayerIconWidget;
class UBillboardComponent;
class UTexture2D;
class UParticleSystem;
class UParticleSystemComponent;
class AFighterPlayerState;
struct FWeakObjectPtr;

class UAkAudioEvent;
class UStaticMeshComponent;
class UMaterialInterface;

UENUM()
enum class EHitPower : uint8
{
	LIGHT = 0,
	MEDIUM,
	SPECIAL
};
UENUM()
enum class EMoveType : uint8
{
	NEUTRAL = 0,
	AIR_NEUTRAL,
	UP,
	DOWN,
	ANTI_AIR,
};

UCLASS()
class NOVAFINAL_API AFighterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFighterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	/*Arrow Stuff*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UBillboardComponent* PlayerArrow;
	UPROPERTY(EditAnywhere)
	UTexture2D* ArrowTexture;
	
	/* A R M S */
#pragma region Hitboxes
	//Left Side
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* LeftHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* LeftArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* LeftShoulder;

	//Right Side
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* RightHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* RightArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* RightShoulder;

	/* L E G S */
	//Left Side
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* LeftThigh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* LeftLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* LeftFoot;
	//Right Side
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* RightThigh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* RightLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* RightFoot;

	/* B O D Y */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* UpperBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UHitboxComponent* LowerBody;
	/* Character Hitbox End*/

#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UStateMachineComponent* SM;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CalculateAggression(); 

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit)override;

	virtual void PossessedBy(AController* NewController) override;
protected:
	UCapsuleComponent* myCap;
public:
	/* Character Movement */
	UFUNCTION(Server, WithValidation, Reliable)
	void DoMoveCharacter(float val);
	UFUNCTION(Client, Reliable)
	void Client_MoveCharacter(float val);
	UFUNCTION()
	void StartJump();
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerWantsToJump();
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Jumping")
	void Client_DoJump();
	UFUNCTION(Server, WithValidation, Reliable)
	void DoJump();
	UFUNCTION()
	void DoJumpAnalog(float val);
	UFUNCTION(Client, Reliable)
	void Client_KBEnd();
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_KBEnd();
	UFUNCTION()
	void HitChar(UPrimitiveComponent* HittingComp,UPrimitiveComponent* HitComp, AActor* HitBy, EHitPower power, float Damage, float KBTime, FVector_NetQuantize KBVec);
	UFUNCTION(Server, WithValidation, Reliable)
	void DoHitChar(UPrimitiveComponent* HittingComp, UPrimitiveComponent* HitComp, AActor* HitBy, EHitPower power, float Damage, float KBTime, FVector_NetQuantize KBVec);


	/* Character Actions To Override */
	UFUNCTION()
		virtual void LightAtk() {}
	UFUNCTION()
		virtual void MedAtk() {}
	UFUNCTION()
		virtual void SpecAtk() {}
	UFUNCTION()
		void Dodge();
	/* Actions end */
	/*Timers Start*/
	UFUNCTION()
		virtual void OnLightAttackEnd() {}
	UFUNCTION()
		virtual void OnMediumAttackEnd() {}
	UFUNCTION()
		virtual void OnSpecialAttackEnd() {}
	UFUNCTION(Client, Reliable)
		void Client_OnDodgeEnd();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DodgeTime = .30f;
	UFUNCTION(Client, Reliable)
		void Client_StopMovement();
	/*Timers End*/

	/* Pause */
	void Pause();
	AFighterPlayerState* PlayerState;
	//AgamePlayUserHUD* HUD;

	UFUNCTION()
	int GetBarsBuilt() { return BarsBuilt; }
protected:
#pragma region BoolToggles
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_LightToggle(ECharacterStates atkType, bool bAtkIsActive);
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_MedToggle(bool bAtkIsActive, EMoveType move);
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_DodgeToggle(bool bIsActive);

#pragma endregion

#pragma endregion

#pragma region Particles
public:
	UFUNCTION(NetMulticast,WithValidation, Reliable)
	void SpawnHitSplat(UPrimitiveComponent* HitComp, EHitPower power);


#pragma endregion



	/*Getters and setters*/
public:
	bool IsFacingRight() const { return bIsFacingRight; }

	

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get dodge", CompactNodeTitle = "Dodge", Keyword = "Get dodge is active"))
		bool CanBeHit() const { return bIsDodging; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get health", CompactNodeTitle = "health", Keyword = "Get health ammount"))
		float GetHealth() const { return Health; }

	UFUNCTION()
		void SetHealth(float newHealth) { Health = newHealth; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Aggression", CompactNodeTitle = "Aggression", Keyword = "Get Aggression ammount"))
		float GetAggression() const { return Aggression; }

	UFUNCTION()
		bool GetHasHit() const { return bHasHit; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get is Winner", CompactNodeTitle = "Winner", Keyword = "Get is Winner is active"))
		bool GetIsWinner() const { return bIsWinner; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get is Dead", CompactNodeTitle = "Dead", Keyword = "Get is Dead is active"))
		bool GetIsDead() const { return bIsDead; }


	/*End getters and setters*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get wants to jump", CompactNodeTitle = "Want Jump", Keyword = "Get wants to jump"))
		bool GetWantToJump() const { return bWantsToJump; }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get in air", CompactNodeTitle = "In Air", Keyword = "Get is in air in the air"))
		bool GetIsInAir() const { return bInAir; }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get hit in air front", CompactNodeTitle = "Hit Air front", Keyword = "Get is in hit air in the air front"))
		bool GetHitAirFront() const { return bHitInAirFront; }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get hit in air back", CompactNodeTitle = "Hit Air back", Keyword = "Get is in hit air in the air back"))
		bool GetHitAirBack() const { return bHitInAirBack; }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get hit on ground", CompactNodeTitle = "Hit Ground", Keyword = "Get is on ground hit  ground "))
		bool GetHitGround() const { return bHitOnGround; }
	UFUNCTION()
		void SetIsFacingRight(bool newIsFacingRight) { bIsFacingRight = newIsFacingRight; }


#pragma region Hitboxes
public:
	//True for right side false for left side
	UFUNCTION(BlueprintCallable, Category = "Hitboxes")
	void Arms(bool isRightSide, bool isActive);
	UFUNCTION(BlueprintCallable, Category = "Hitboxes")
	void Legs(bool isRightSide, bool isActive);
	UFUNCTION(BlueprintCallable, Category = "Hitboxes")
	void Body(bool isActive);
	UFUNCTION(BlueprintCallable, Category = "Hitboxes")
	void HeadHitbox(bool isActive);

#pragma endregion




//==========Paul's Wwise Stuff================

	//AK Events:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent * jumpSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent * landSound;

	bool isJumping = false;
	bool isLandingSoundTriggered = false;

public:

#pragma region WinLoseScreenCode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets|Classes")
	TSubclassOf<UWinLoseScreen> WinLoseScreenClass;

	UWinLoseScreen* WinLoseScreen;
	UPROPERTY(Replicated)
	ECondition Condition = ECondition::Undecided;

	UPROPERTY(Replicated)
	bool IsConditionDecided = false;

	//Using netmulticast to access the widget by IsLocallyControlled:
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void CreateWinLoseWidget(ECondition cond);
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void DisablePawnInput();

	UPROPERTY(Replicated)
	bool bIsDead = false;

//======= Win Lose Screen: ============================================
	UFUNCTION(Server, WithValidation, Reliable)
	void Trigger_HealthZero(AFighterBase* fb);

	UPROPERTY(Replicated)
	bool bIsMovementEnabled = false;

	UPROPERTY(Replicated)
	bool bIsWinner = false;
#pragma endregion



public:

#pragma region Properties

	/*Bools for replication in animations*/
protected:
	UPROPERTY(Replicated)
		bool bWantsToJump = false;
	UPROPERTY(Replicated)
		bool bInAir = false;
	UPROPERTY(Replicated)
		bool bIsFacingRight = true;
	UPROPERTY(Replicated)
		bool bIsDodging = false;
	UPROPERTY(Replicated)
		bool bHitInAirFront = false;
	UPROPERTY(Replicated)
		bool bHitInAirBack = false;
	UPROPERTY(Replicated)
		bool bHitOnGround = false;
	float KnockbackDelay = 0;
	UPROPERTY(Replicated)
		bool bIsInKnockback = false;
	/*Individual Properties*/
	UPROPERTY(EditAnywhere, Replicated)
		float Health = 100.0f;
	UPROPERTY(EditAnywhere, Replicated)
		int BarsBuilt = 0;
	UPROPERTY(EditAnywhere, Replicated)
		float Aggression = 0.0f;
	UPROPERTY(Replicated)
		bool bIsHitting = false;
	UPROPERTY(Replicated)
		bool bHasHit = false;
	UPROPERTY(Replicated)
		float DamageDone = 0.0f;
	UPROPERTY(Replicated)
		float AttacksThrown = 0.0001f;
	UPROPERTY(Replicated)
		float AttacksHit = 0.0f;
	UPROPERTY()
		bool bDebugStateMachine = false;
	UPROPERTY(EditAnywhere)
		FVector_NetQuantize DodgeDistance = FVector_NetQuantize(0.0f, 2000.0f, -300.0f);
	UPROPERTY()
		bool bHasDodged = false;


	/*Timer handlers*/


	UPROPERTY(EditAnywhere, Replicated)
		TArray<UParticleSystem*> LightHitSplat;
	UPROPERTY(EditAnywhere, Replicated)
		TArray<UParticleSystem*> MedHitSplat;
	UPROPERTY(EditAnywhere, Replicated)
		UParticleSystem* SpecHitSplat = nullptr;

	UPROPERTY(EditAnywhere, Replicated)
		UParticleSystemComponent* DashRightParticleComponent;
	UPROPERTY(EditAnywhere, Replicated)
		UParticleSystemComponent* DashLeftParticleComponent;
	UPROPERTY(EditAnywhere, Replicated)
		UParticleSystem* DashLeft = nullptr;
	UPROPERTY(EditAnywhere, Replicated)
		UParticleSystem* DashRight = nullptr;
	UPROPERTY(EditAnywhere, Replicated)
		UParticleSystem* LandEffect = nullptr;
	UPROPERTY(EditAnywhere, Replicated)
		UParticleSystemComponent* SpecialAttack;


public:
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_LaunchChar(FVector LaunchVelocity);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SpawnGameplayWidgets();

	//Wwise
	UFUNCTION(Server, WithValidation, Reliable)
	void Multicast_SetSliderVals();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event|Classes")
		UAkAudioEvent * MainMenuEvent;

	//UFUNCTION(NetMulticast, WithValidation, Reliable)
	void AdjustSFXSlider();

	bool IsSFXSliderAdjusted = false;

	/*Combat Anim Booleans*/
	UPROPERTY(Replicated)
		bool bLightAtk = false; //grounded light attack
	UPROPERTY(Replicated)
		bool bMedAtk = false; //Grounded med attack
	UPROPERTY(Replicated)
		bool bMedLauncher = false; // Launcher = Anti air for med attacks
	UPROPERTY(Replicated)
		bool bLightAtkAir = false; //Standard air attack
	UPROPERTY(Replicated)
		bool bMedAtkAir = false; //standard air launcher
	UPROPERTY(Replicated)
		bool bMedAirDown = false; //Downward air launcher
	UPROPERTY(Replicated)
		bool bMedAirUp = false; //Upwards med air launcher
	UPROPERTY(Replicated)
		bool bSpecAtk = false;



	//Timers
	FTimerHandle TimerHandle_Knockback;

	FTimerHandle TimerHandle_LightPunch;

	FTimerHandle TimerHandle_LightAir;

	FTimerHandle TimerHandle_MedGround;
	FTimerHandle TimerHandle_MedLauncher;
	FTimerHandle TimerHandle_MedAirNeutral;
	FTimerHandle TimerHandle_MedAirUp;
	FTimerHandle TimerHandle_MedAirDown;

	FTimerHandle TimerHandle_SpecialAttack;

	FTimerHandle TimerHandle_Dodge;


	//Getters for the animation tree. 
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get light attack", CompactNodeTitle = "Light Atk", Keyword = "Get light attack is active"))
		bool LightAttacking() const { return bLightAtk; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get medium attack", CompactNodeTitle = "Med Atk", Keyword = "Get medium attack is active"))
		bool MedAttacking() const { return bMedAtk; }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get medium launcher", CompactNodeTitle = "Med Launch", Keyword = "Get medium launch is active"))
		bool MedLauncher() const { return bMedLauncher; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get light air attack", CompactNodeTitle = "Air Light Atk", Keyword = "Get light air attack is active"))
		bool LightAirAttacking() const { return bLightAtkAir; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get medium air attack", CompactNodeTitle = "Air Med Atk", Keyword = "Get medium attack is active air"))
		bool MedAirAttacking() const { return bMedAtkAir; }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get medium up air attack", CompactNodeTitle = "Air Med Up", Keyword = "Get medium up attack is active air"))
		bool MedAirUpAtk() const { return bMedAirUp; }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get medium down air attack", CompactNodeTitle = "Air Med Down", Keyword = "Get medium down attack is active air"))
		bool MedAirDownAtk() const { return bMedAirDown; }


	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get heavy attack", CompactNodeTitle = "Heavy Atk", Keyword = "Get heavy attack is active"))
		bool SpecAttacking() const { return bSpecAtk; }

	UFUNCTION(Server, WithValidation, Reliable)
		void Server_StopAllAnims();

	//Wwise dashes:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
	UAkAudioEvent * DashStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
	UAkAudioEvent * DashStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
	UAkAudioEvent * BarrierHit;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Wwise_PlayDashStart();	
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Wwise_PlayDashStop();
	//UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Wwise_PlayBarrier();

	UPROPERTY(Replicated)
	bool IsDashStopPlayed;

	UFUNCTION(Server, WithValidation, Reliable)
	void SetIsDashStopPlayed(bool isPlayed);

	UFUNCTION(Server, WithValidation, Reliable)
	void Trigger_PlayDashStop();



	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> ArrowMats;

	UPROPERTY(ReplicatedUsing = OnRep_SetArrowMaterial)
		UMaterialInterface* ArrowMat;

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void SetArrowMaterial(UStaticMeshComponent* SMC);
	/*Colour*/
	UFUNCTION()
		virtual void OnRep_SetArrowMaterial();

	UPROPERTY(Replicated)
		UStaticMeshComponent* ArrowMeshComp = nullptr;
};