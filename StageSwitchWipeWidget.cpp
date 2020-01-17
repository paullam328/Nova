// Copywrite Shapeshifter 2019

#include "StageSwitchWipeWidget.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Image.h"
#include "TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"



UStageSwitchWipeWidget::UStageSwitchWipeWidget(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UStageSwitchWipeWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	/*FString thingyToPrint = "im still here";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *thingyToPrint));*/

	//if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) != "StageTransitionMap")
	//{
		if (isOnTickWipeActivated)
		{
			Wipe(true);
		}
	//}
	//else
	//{
	//	DynMat->SetScalarParameterValue(Disolve, 2);
	//}

	if (FlashingAnim != nullptr)
	{
		PlayFlashingAnim(DeltaTime);
	}
}

void UStageSwitchWipeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Logo = (UImage*)GetWidgetFromName(TEXT("Logo"));
	DynMat = Logo->GetDynamicMaterial();
	Logo->SetBrushFromMaterial(DynMat);


	//Comment removal:

	DynMat->SetScalarParameterValue(Disolve, -2.8);


	//FactText = (UTextBlock*)GetWidgetFromName(TEXT("Fact"));
	//Text = (UTextBlock*)GetWidgetFromName(TEXT("Title"));
	//FactText->SetVisibility(ESlateVisibility::Hidden);
	//Text->SetVisibility(ESlateVisibility::Hidden);

	DissolveInAnim = GetWidgetTreeOwningClass()->Animations[0];
	FlashingAnim = GetWidgetTreeOwningClass()->Animations[1];
	DissolveOutAnim = GetWidgetTreeOwningClass()->Animations[2];
}
/*
void UStageSwitchWipeWidget::Wipe()
{
	if (DynMat->K2_GetScalarParameterValue(Disolve) < 2 && DynMat->K2_GetScalarParameterValue(Disolve) > -3 && !hasWiped)
	{
		float val = DynMat->K2_GetScalarParameterValue(Disolve) + wipeTime;
		DynMat->SetScalarParameterValue(Disolve, val);
	}
	else if(DynMat->K2_GetScalarParameterValue(Disolve) > -3)
	{
		hasWiped = true;
		float val = DynMat->K2_GetScalarParameterValue(Disolve) - wipeTime;
		DynMat->SetScalarParameterValue(Disolve, val);
		
		if (DynMat->K2_GetScalarParameterValue(Disolve) == -2.7)
		{
			this->RemoveFromParent();
		}
	}
	
}
*/
 void UStageSwitchWipeWidget::Wipe(bool isWipeAppear)
{
	 if (isWipeAppear)
	 {
		 //Appear:
		 if (DynMat->K2_GetScalarParameterValue(Disolve) < 1.90f)
		 {
			 float val = DynMat->K2_GetScalarParameterValue(Disolve) + wipeTime * wipeSpeed;
			 DynMat->SetScalarParameterValue(Disolve, val);
			 if (!hasDissolved)
			 {
				 PlayDissolveInAnim();
				 hasDissolved = true;
			 }
		 }
		 else
		 {
			 if (!isTextInitialized)
			 {
				 //GenerateFactText();
				 //FactText->SetVisibility(ESlateVisibility::Visible);
				 //Text->SetVisibility(ESlateVisibility::Visible);
				 //isTextInitialized = true;



				 isOnTickWipeActivated = false;
			 }
		 }
	 }
	 else
	 {
		 //Disappear:
		 if (DynMat->K2_GetScalarParameterValue(Disolve) > 0)
		 {
			 float val = DynMat->K2_GetScalarParameterValue(Disolve) - wipeTime;
			 DynMat->SetScalarParameterValue(Disolve, val);
			 //FactText->SetVisibility(ESlateVisibility::Hidden);
			//Text->SetVisibility(ESlateVisibility::Hidden);
			 if (!hasUndissolved)
			 {
				 PlayDissolveOutAnim();
				 hasUndissolved = true;
			 }

		 }
		 else
		 {
			 //Delete this widget:
			 hasWiped = true;
			 if (this != nullptr)
			 {
				 SetVisibility(ESlateVisibility::Hidden);
				 if (this->GetParent() != nullptr)
				 {
					 this->RemoveFromParent();
				 }
			 }
		 }
	 }
}

void UStageSwitchWipeWidget::GenerateFactText()
{
	FString FactTextStr = "";
	switch (UKismetMathLibrary::RandomInteger(10))
	{
		case 0:
			FactTextStr = "If you see Derek, make sure to get your ice cream!";
			break;
		case 1:
			FactTextStr = "Our Audio Programmer is a toxic mario kart player.";
			break;
		case 2:
			FactTextStr = "Our Network Engineer has a crush on Ricardo Milos.";
			break;
		case 3:
			FactTextStr = "Appearnetly we're \"too loud\".";
			break;
		case 4:
			FactTextStr = "Our only source of motivationm is memes.";
			break;
		case 5:
			FactTextStr = "Bottom Text";
			break;
		case 6:
			FactTextStr = "We live in a society.";
			break;
		case 7:
			FactTextStr = "Okay la.";
			break;
		case 8:
			FactTextStr = "Our Project Manager cries whenever he loses in Super Smash Brothers.";
			break;
		case 9:
			FactTextStr = "Our AI Programmer can hold a slav squat for over 10 hours.";
			break;
		case 10:
			FactTextStr = "Our Artist, wait we have an artist?";
			break;
		default:
			break;
	}
	//FactText->SetText(FText::FromString(*FactTextStr));
}


void UStageSwitchWipeWidget::SetDisolve(float dis)
{
	DynMat->SetScalarParameterValue(Disolve, dis);
}

void UStageSwitchWipeWidget::PlayDissolveInAnim()
{
	if (DissolveInAnim != nullptr)
	{
		PlayAnimation(DissolveInAnim, 0, 1);
	}
}

void UStageSwitchWipeWidget::PlayFlashingAnim(float DeltaTime)
{
	if (FlashingAnim != nullptr)
	{
		if (FlashAnimTimer > 1)
		{
			PlayAnimation(FlashingAnim, 0, 1);
			FlashAnimTimer = 0;
		}
		else
		{
			FlashAnimTimer += DeltaTime;
		}
	}
}

void UStageSwitchWipeWidget::PlayDissolveOutAnim()
{
	if (DissolveOutAnim != nullptr)
	{
		PlayAnimation(DissolveOutAnim, 0, 1);
	}
}

void UStageSwitchWipeWidget::SetTransitionScreenTransparent()
{
	if (ensure(this != nullptr))
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}