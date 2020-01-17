// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "FighterBase.h"
#include "DemarkBase.generated.h"

/**
 * 
 */
class UAkComponent;
class UAkAudioEvent;

/**
 *
 */
class UHitboxComponent;
class ADemark_Special;

UCLASS()
class NOVAFINAL_API ADemarkBase : public AFighterBase
{
	GENERATED_BODY()

public:
	ADemarkBase(const FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void PreInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

#pragma region HitboxStuff

public:
	/* Character Hitbox Colliders */
	UFUNCTION()
		void SetupHitboxComponents();
	UFUNCTION()
		void MakeHitboxesVisible(bool setting);

#pragma endregion
#pragma region HitReactionsFunctionsAndTimers
public:
	/*Hit reaction Functions and timers*/
	UFUNCTION(Server, WithValidation, Reliable)
	void OnEndHit();
protected:
	/*Hit reaction Functions and timers end*/
	FTimerHandle PostHitTimer;
	/* Hitbox Reactions */
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION(Server, WithValidation, Reliable)
		void DoOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/* Hitbox Reactions End */
#pragma endregion
#pragma region ActionsAndTimers

public:
	/* Character Actions	*/
	virtual void LightAtk() override;
	virtual void MedAtk() override;
	virtual void SpecAtk() override;
	/* Actions end*/


	/*Timers Start*/
	UFUNCTION(Client, Reliable)
	void Client_OnLightAttackEnd();
	UFUNCTION(Client, Reliable)
	void Client_OnMediumAttackEnd();
	UFUNCTION(Client, Reliable)
	void Client_OnSpecialAttackEnd();
	

	/*Timers End*/
#pragma endregion
#pragma region Colour
public:
	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> CharColours;

	UPROPERTY(ReplicatedUsing = OnRep_SetCharColour)
	UMaterialInterface* CharColour;
	UFUNCTION(Server, WithValidation, Reliable)
	void SERVER_SetCharColor(int index);
	/*Colour*/
	UFUNCTION()
	virtual void OnRep_SetCharColour();

#pragma endregion



#pragma region Properties
public:
	/*Individual Properties*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		int PlayerNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShowHitboxes = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LightAtkTime = .45f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LightAirTime = .55f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MedAtkTime = .75f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MedAirTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MedLauncherTime = .75f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MedAirUpTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MedAirDownTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MedAirNeutralTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpecialTime = 1.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MedLaunchDistance = 3000.f;
	/*Individual Properties End*/
	FTimerHandle TimerHandle_ShootProj;
#pragma endregion
#pragma region Wise
public:
	//==========Paul's Wwise Stuff================

	//AK Events:

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable, Category = "Anim Trigger Sound")
		void PlayLightSound();

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable, Category = "Anim Trigger Sound")
		void PlayMedSound();

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable, Category = "Anim Trigger Sound")
		void PlayLightAirSound();

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable, Category = "Anim Trigger Sound")
		void PlayMedAirSound();

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable, Category = "Anim Trigger Sound")
		void PlaySpSound();

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable, Category = "Anim Trigger Sound")
		void PlaySpAtkAnimSound();

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable, Category = "Anim Trigger Sound")
		void PlayDeadSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * lightAtkSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * medAtkSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * lightAirAtkSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * medAirAtkSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * spAtkSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * spAtkAnimSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * lightHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * medHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * spAtHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Event|Classes")
		UAkAudioEvent * deadSound;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void PlayLightHitSound();
	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void PlayMedHitSound();
	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void PlaySpecHitSound();

	//=============================================
#pragma endregion


	/* Special Attack Projectile */
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADemark_Special> ProjectileRight;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADemark_Special> ProjectileLeft;
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SpecToggle(bool bAtkIsActive);
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SpawnSpecActor();

	/* Move specific Knockbacks */
	UPROPERTY(EditAnywhere)
		FVector_NetQuantize LightGroundKB = FVector_NetQuantize(0.0f, 1000.f, 1000.f); //should be linkable to medium

	UPROPERTY(EditAnywhere)
		FVector_NetQuantize LightAirKB = FVector_NetQuantize(0.0f, 50.f, 1500.0f); //Should be a link to 5m or 2M

	UPROPERTY(EditAnywhere)
		FVector_NetQuantize MedGroundKB = FVector_NetQuantize(0.0f, 500.0f, 3500.0f); //This should be jump recombable

	/* ANTI AIR MOVE */
	UPROPERTY(EditAnywhere)
		FVector_NetQuantize MedGroundUpKb = FVector_NetQuantize(0.0f, 500.0f, 3500.0f); // Should go up and away.


	UPROPERTY(EditAnywhere)
		FVector_NetQuantize MedAirKB = FVector_NetQuantize(0.0f, 2000.f, 0.0f); //Straight away. Can recombo off wall bounce with med air up?

	UPROPERTY(EditAnywhere)
		FVector_NetQuantize MedAirDownKB = FVector_NetQuantize(0.0f, 1000.f, -2000.f); //Smash down and away


	UPROPERTY(EditAnywhere)
		FVector_NetQuantize MedAirUpKB = FVector_NetQuantize(0.0f, 1500.f, 1500.f); //Smash up and away as finish

	UPROPERTY(EditAnywhere)
		FVector_NetQuantize SpecKB = FVector_NetQuantize(0.0f, 2500.f, 500.f); //Special KB Should be up and away



	UPROPERTY(EditAnywhere)
		float LightGroundKBTime = .70f;
	UPROPERTY(EditAnywhere)
		float MedGroundKBTime = 1.2f;
	UPROPERTY(EditAnywhere)
		float MedLauncherKBTime = 1.5f;
	UPROPERTY(EditAnywhere)
		float LightAirKBTime = .55f;
	UPROPERTY(EditAnywhere)
		float MedAirNeutralKBTime = 1.f;
	UPROPERTY(EditAnywhere)
		float MedAirDownKBTime = 1.25f;
	UPROPERTY(EditAnywhere)
		float MedAirUpKBTime = 1.f;
	UPROPERTY(EditAnywhere)
		float SpecKBTime = 2.f;

	//tester function
	UFUNCTION()
		void TestFunc(float val);
};